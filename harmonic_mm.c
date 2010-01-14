#include "assert.h"
#include "harmonic.h"

gsl_matrix * D_inv, * C;
double * m;
gsl_vector * fx, * fu, * ftmp;

int main ( void )
{
  ODE_solver * s;
  int M = 35, K = 0, N = 2*(M+K)+1,i;
  H_DOUBLE T =1.e100;
  H_DOUBLE x0 = 0., x1 = PI, x;
  H_DOUBLE t_error = 1.e-8;
  h_basis_functions * basis = h_basis_finite_difference_5_function_init();
  gsl_odeiv_step_type * stepper = gsl_odeiv_step_rkf45;
  gsl_matrix * D = gsl_matrix_alloc(N,N);
  gsl_permutation * p = gsl_permutation_alloc(N);
  FILE * file;

  /* Alokacja pamięci i inicjalizacja oraz odwrócenie macierzy przy
     użyciu GSL */
  m = malloc( N*sizeof(double) );
  D_inv = gsl_matrix_alloc(N,N);
  C = gsl_matrix_alloc(N,N);
  fx = gsl_vector_alloc(N);
  fu = gsl_vector_alloc(N);
  ftmp = gsl_vector_alloc(N);

  gsl_matrix_set_zero( C );
  gsl_matrix_set_zero( D );

  for (i = 1; i < N; i++ )
    {
      gsl_matrix_set(D, i, i, -2.);
      gsl_matrix_set(D, i, i-1, 1.);
      gsl_matrix_set(D, i-1, i, 1.);
    }

  gsl_matrix_set(D, N-1, N-1, 1.);
  gsl_matrix_set(D, 0, 0, 1.);

  gsl_linalg_LU_decomp(D, p, &i );
  gsl_linalg_LU_invert(D, p, D_inv);

  /* inicjalizacja struktury przechowującej informacje dot. dymulacji */
  s = ODE_solver_init ( 2*N+1, /*rk=*/ 1, T, x0, x1, t_error, basis, ODE_set, stepper );
  /* inicjalizacja modułów obrazujących przebieg symulacji, pierwszy
     argument to odstęp (mierzony czasem obliczeniowym) w jakim mają
     być wywoływane kolejne moduły */
  /* modul do wizualizacji wykresu fcji w czasie rzeczywistym */
  /* ODE_modules_add ( s, ODE_module_plot_init( 1.e-2 ) ); */
  /* modul do drukowania w konsoli czasu symulacji */
  ODE_modules_add ( s, ODE_module_print_time_init ( 1. ) );
  /* modul do wpisywania do pliku log/info_1/log001.dat szeregu
     informacji dot. funkcji, w kolejnosci sa to:
     tau, t, u[1], x[1], du(0,tau)/dx, g, *dtau, 0. */
  ODE_modules_add ( s, ODE_module_info_1_init( .1, N ) );
  /* modul wpisywania profili fcji do katalogu log/snapshot */
  ODE_modules_add ( s, ODE_module_snapshot_init( 10. ));
  /* ODE_modules_add ( s, ODE_module_movie_maker_init( 0.) ); */

  /* inicjalizacja danych poczatkowych */
  /* file = fopen( "bisection.dat", "r" ); */

  /* s->state->f[1]=PI; */
  /* s->state->f[1+N]=0.; */
  /* s->state->f[N]=PI; */
  /* s->state->f[2*N]=PI; */

  /* for ( i = 1; i < M; i++ ) { */
  /*   fscanf(file, "%lf %lf %lf", s->state->f + i+1+N,s->state->f + i+1, &x); */
  /*   s->state->f[N-i]=s->state->f[i+1]; */
  /*   s->state->f[i+1+N]*=1.e-5; */
  /*   s->state->f[2*N-i]=PI-s->state->f[i+1+N]; */
  /*   /\* printf("%lf %lf\n", s->state->f[i+1+M],s->state->f[i+1]); *\/ */
  /* } */

  /* for ( i = M; i < M+K; i++) */
  /*   { */
  /*     s->state->f[1+i]=s->state->f[M]; */
  /*     s->state->f[N-i]=s->state->f[M]; */
  /* 	/\* s->state->f[M]; *\/ */
  /*     s->state->f[i+1+N]= */
  /* 	s->state->f[M+N]+ */
  /* 	(PI/2.-s->state->f[M+N]) */
  /* 	/(pow(.01*(double)(K+1),4)) */
  /* 	*(pow(.01*(double)(i-M+1),4)); */
  /*     s->state->f[2*N-i]=PI-s->state->f[i+1+N]; */
  /*   } */

  /* s->state->f[(N+1)/2]=s->state->f[(N+1)/2-1]; */
  /* s->state->f[(N+1)/2+N]=PI/2.; */
  /* fclose(file); */

  x=x0;

  for ( i = 0; i < N; i++ ) {
    s->state->f[i+1+N]=x;
    /* s->state->f[i+1]=x/x1*PI+sin(x/x1*PI); */
    s->state->f[i+1]=x+sin(x);
    x += (x1-x0)/(N-1);
  }
  s->state->f[0]=0.;

  file = fopen ( "test.dat", "w" );
  for ( i = 0; i < 2*N+1; i++ )
    fprintf(file, "%i %.15f\n", i, s->state->f[i]);

  fclose( file );

  /* ponizsza funkcja pcha wartosci poczatkowe w czasie i uruchamia
     poszczegolne moduly */
  ODE_solve ( s );

  /* uwolnienie zaalokowanej pamieci */
  ODE_solver_free( s );

  free(m);
  gsl_matrix_free( D_inv );
  gsl_matrix_free( D );
  gsl_matrix_free( C );
  gsl_vector_free( fx );
  gsl_vector_free( fu );
  gsl_vector_free( ftmp );

  return 0;
}

/* W ponizszej funkcji definiujemy uklad rownan */
void ODE_set ( void * solver,
	       H_DOUBLE t,
	       H_DOUBLE * y,
	       H_DOUBLE * f )
{
  ODE_solver * s = (ODE_solver*)solver;
  int i,N = (s->params->Nx-1)/2;
  /* stan wszystkich zmiennych jest przechowywany w tablicy 2*N + 1
     elementowej "y", gdzie y[0] to czas fizyczny, y[1+N] do y[2*N] to
     punkty siatki w przestrzeni fizycznej, a y[1] do y[N] to wartości
     funkcji w odpowiednich punktach */
  H_DOUBLE * ui = y + 1;
  H_DOUBLE * xi = y + 1 + N;
  H_DOUBLE k = 7.,u,x,du,ddu,Mxi,de,epsilon,gt;
  H_DOUBLE dt = *(s->state->dt);

  /* definicje zmiennych pomocniczych */
  epsilon = 1.e-4;
  de = 1./(N-1);
  M_calc( ui, xi, m, N );

  gt = g( y, N );
    /* gt = 1.; */

  assert(!isnan(gt));
  assert(!isnan(y[0]));

  /* warunek na zatrzymanie ewolucji */
  if( gt*dt < 1.e-15 /* || fabs(D1(ui, xi, 0, N)) < 10000 */)
    {
      printf("STOP: gt < 1.e-15\n");
      s->state->status = SOLVER_STATUS_STOP;
    }

  for ( i = 1; i < N-1; i++) {
    u=ui[i];
    x=xi[i];
    /* obliczenie pochodnych w punkcie "i" */
    du=D1(ui,xi,i,N);
    ddu=D2(ui,xi,i,N);

    assert(!isnan(du));
    assert(!isnan(ddu));

    /* prawa strona rownania macierzowego */
    Mxi=((m[i+1]+m[i])*(xi[i+1]-xi[i])-(m[i]+m[i-1])*(xi[i]-xi[i-1]))/2./de/de;
    assert(!isnan(Mxi));
    /* Mxi=0.; */

    gsl_vector_set(fu, i,
  		   gt*(ddu+(k-1.)/x*du-(k-1.)/2.*sin(2.*u)/x/x));
    gsl_vector_set(ftmp, i,
  		   gt/epsilon*Mxi);
    gsl_matrix_set(C, i, i, -du);
  }

  /* wymnozenie prawej strony rownania macierzowego przez odwrotnosc
     macierzy */
  gsl_blas_dsymv (CblasUpper, -1., D_inv, ftmp, 0., fx); /* D = -d2/de2 */
  gsl_blas_dgemv (CblasNoTrans, -1., C, fx, 1., fu);	/* C = -du/dx */

  /* przepisanie wynikow do tablicy pochodnej czasowej */

  for ( i = 1; i < N-1; i++) {
    f[i+1]=gsl_vector_get(fu,i);
    f[i+N+1]=gsl_vector_get(fx,i);
  }
  f[0]=gt;

  /* warunki brzegowe */
  f[1]=0.;
  f[N]=0.;
  f[N+1]=0.;
  f[2*N]=0.;
}

/* obliczanie pierwszej pochodnej */
double D1 ( double * u, double * x, int i, int N )
{
  double du;

  /* for ( i = 0; i < N; i++ ) */
  /*   printf("%i %.15f %.15f\n", i, x[i], u[i]); */

  if( i > 0 && i < N-1 )
    du=
      (u[-1+i]-u[i])/(x[-1+i]-x[i])+
      (-u[-1+i]+u[1+i])/(x[-1+i]-x[1+i])+
      (u[i]-u[1+i])/(x[i]-x[1+i]);
  else if( i == 0 )
    du=
      (u[1]-u[0])/(x[1]-x[0])
      +(u[2]-u[0])/(x[2]-x[0])
      -(u[2]-u[1])/(x[2]-x[1]);
  else if( i == N-1 )
    du=
      (-u[-3+N]+u[-2+N])/(x[-3+N]-x[-2+N])
      +(u[-3+N]-u[-1+N])/(x[-3+N]-x[-1+N])
      +(u[-2+N]-u[-1+N])/(x[-2+N]-x[-1+N]);
  else du = 0.;

  /* printf("D1: i=%i du=%.15f\n", i, du); */
  return du;
}

/* obliczanie drugiej pochodnej */
double D2 ( double * u, double * x, int i, int N )
{
  if( i > 0 && i < N-1 )
    return ((u[i+1]-u[i])/(x[i+1]-x[i])
	    -(u[i]-u[i-1])/(x[i]-x[i-1]))
      *2./(x[i+1]-x[i-1]);
  else if (i == 0)
    return (2.*
	    ( (u[i]-u[2+i])/(x[i]-x[2+i])
	      +(-u[1+i]+u[2+i])/(x[1+i]-x[2+i]))
	    )/(x[i]-x[1+i]);
  else if (i==N-1)
    return (2.*
	    ( (u[-2+i]-u[i])/(x[-2+i]-x[i])
	      +(-u[-1+i]+u[i])/(x[-1+i]-x[i])
	      )
	    )/(x[-2+i]-x[-1+i]);
  else return 0.;
}

/* funkcja definiujaca transformacje Sundmana
   dt/dtau=g(u)=0.01/(du/dx(0,tau))^2 */
double g ( double * y, int N )
{
  /* return 0.01*pow(fabs(D1(y+1,y+1+N,0,N))+fabs(D1(y+1,y+1+N,N-1,N)),-2); */
  double k = 7.;
  H_DOUBLE * ui = y + 1;
  H_DOUBLE * xi = y + 1 + N;
  double du=D1(ui,xi,1,N),ddu=D2(ui,xi,1,N);
  double x=xi[1];
  double u=ui[1];
  double ut=(ddu+(k-1.)/tan(x)*du-(k-1.)/2.*sin(2.*u)/sin(x)/sin(x));
  /* printf("du=%f, ut=%f, x=%f\n",du,ut,x); */
  return .01*(fabs(x*du/ut));
}

/* funkcja rozkladu punktow fizycznej siatki (
   M(x)=du/dx+sqrt(d2u/dx2) ) */
void M_calc ( double * u, double * x, double * M, int N )
{
  int i;
  double Mtot=0.;

  for ( i = 1; i < N-1; i++)
    {
      M[i]=2.*fabs( D1( u, x, i, N ) )
	+ sqrt( fabs( D2( u, x, i, N ) ) );
      Mtot+=(M[i]*(x[i+1]-x[i-1])/2.);

      /* printf("M_calc: i=%i, M[i]=%.15f\n", i, M[i]); */
      /* printf("M_calc: M[i]-M[i]=%.15f\n", M[i]-M[i]); */

      assert( M[i]-M[i]==0. );
      assert( M[i] >= 0 );
    }

  M[0]=fabs( D1( u, x, 0, N ) )
	+ sqrt( fabs( D2( u, x, 0, N ) ) );
  M[N-1]=fabs( D1( u, x, N-1, N ) )
	+ sqrt( fabs( D2( u, x, N-1, N ) ) );

  Mtot+=M[0]*(x[1]-x[0])+M[N-1]*(x[N-1]-x[N-2]);

  for ( i = 0; i < N; i++ )
    {
      M[i]/=Mtot;
      /* M[i]+=.01*(1.+sin(x[i])); */
    }
}
