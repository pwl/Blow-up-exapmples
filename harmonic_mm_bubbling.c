#include "assert.h"
#include "harmonic.h"

gsl_matrix * D_inv, * C;
double * m;
gsl_vector * fx, * fu, * ftmp;

int main ( void )
{
  ODE_solver * s;
  int M = 67, K = 0, N = M+K, i;
  H_DOUBLE a,b,t;
  H_DOUBLE T =1.e100;
  H_DOUBLE x0 = 0., x1 = 100., x;
  H_DOUBLE t_error = 1.e-12;
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
  ODE_modules_add ( s, ODE_module_plot_sin_init( 100. ) );
  /* ODE_modules_add ( s, ODE_module_plot_init( 1.e-3 ) ); */
  /* modul do drukowania w konsoli czasu symulacji */
  ODE_modules_add ( s, ODE_module_print_time_init ( 0. ) );
  /* modul do wpisywania do pliku log/info_1/log001.dat szeregu
     informacji dot. funkcji, w kolejnosci sa to:
     tau, t, u[1], x[1], du(0,tau)/dx, g, *dtau, 0. */
  ODE_modules_add ( s, ODE_module_info_1_init( 1., N ) );
  /* modul wpisywania profili fcji do katalogu log/snapshot */
  ODE_modules_add ( s, ODE_module_snapshot_init( 10. ));
  /* ODE_modules_add ( s, ODE_module_movie_maker_init( 50.) ); */

  /* inicjalizacja danych poczatkowych */

  /* Z PLIKU */

  file = fopen( "bisection.dat", "r" );

  for ( i = 1; i < M; i++ ) {
    fscanf( file, "%lf %lf %lf",
	    s->state->f + i + 1 + N,
	    s->state->f + i + 1,
	    &x );
    x = s->state->f[i+N+1];
    s->state->f[i+1]*=x*(1.+1.e-2);
    /* s->state->f[N-i]=s->state->f[i+1]; */
    /* s->state->f[i+1+N]*=1.e-7; */
    /* s->state->f[2*N-i]=PI-s->state->f[i+1+N]; */
  }

  fclose(file);

  for ( i = M; i < M+K; i++)
    {
      t=(double)(i-M+1)/(double)(K);
      a=s->state->f[M+N];
      b=x1;

      /* x=a+(b-a)*t; */
      x=a/pow(a/b,t);

      s->state->f[i+1+N]=x;
      s->state->f[1+i]=s->state->f[M]/s->state->f[M+N]*x;
    }

  s->state->f[1]=0.;
  s->state->f[1+N]=0.;

  /* PRZEZ FUNKCJE */

  /* x=x0; */

  /* for ( i = 0; i < N; i++ ) { */
  /*   s->state->f[i+1+N]=x; */
  /*   /\* s->state->f[i+1]=x/x1*PI+sin(x/x1*PI); *\/ */
  /*   s->state->f[i+1]=(sin(3.*x)+3.*x)*sin(x); */
  /*   x += (x1-x0)/(N-1); */
  /* } */
  /* s->state->f[0]=0.; */

  /* test danych poczatkowych */

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
  H_DOUBLE k = 3,u,x,du,ddu,Mxi,de,epsilon,gt;

  /* definicje zmiennych pomocniczych */
  epsilon = 1.e-1;
  de = 1./(N-1);
  M_calc( ui, xi, m, N );
  gt = g( y, N );
  /* gt = 1.e-4; */

  assert(gt-gt==0.);
  /* assert(!isnan(y[0])); */

  /* warunek na zamrozenie transf. sundmana */
  if( gt <= 1e-15 || gt-gt!=0.)
    gt=1.e-15;

  /* warunek na zatrzymanie symulacji */
  /* if( fabs(ui[1]/xi[1] - ui[N-2]/xi[N-2]) < 1.e-2) */
  /*   s->state->status=SOLVER_STATUS_STOP; */

  /* if( gt < 1e-15 || gt-gt!=0.) */
  /*   s->state->status = SOLVER_STATUS_STOP; */

  for ( i = 1; i < N-1; i++) {
    u=ui[i];
    x=xi[i];
    /* obliczenie pochodnych w punkcie "i" */
    du=D1(ui,xi,i,N);
    ddu=D2(ui,xi,i,N);

    /* assert(du-du==0.); */
    /* assert(ddu-ddu==0.); */

    /* prawa strona rownania macierzowego */
    Mxi=((m[i+1]+m[i])*(xi[i+1]-xi[i])-(m[i]+m[i-1])*(xi[i]-xi[i-1]))/2./de/de;
    /* assert(!isnan(Mxi)); */
    /* Mxi=0.; */
    /* printf("i=%i, Mxi[i]=%f, m[i-1]=%f, m[i]=%f, m[i+1]=%f\n",i,Mxi,m[i-1],m[i],m[i+1]); */
    /* printf("i=%i, x[i-1]=%f, x[i]=%f, x[i+1]=%f\n",i,xi[i-1],xi[i],xi[i+1]); */

    if( gt <= 1e-14 || gt-gt!=0.)
      Mxi=0.;

    /* assert(Mxi-Mxi==0.); */

    gsl_vector_set(fu, i,
  		   gt*(ddu/* +u */-sin(2.*u/x)/x));
    gsl_vector_set(ftmp, i,
  		   gt/epsilon*Mxi);
    gsl_matrix_set(C, i, i, -du);
  }

  /* printf("i=%i, Mxi[i]=%f, m[i-1]=%f, m[i]=%f, m[i+1]=%f\n",i,Mxi,m[i-1],m[i],m[i+1]); */
  /* printf("i=%i, x[i-1]=%f, x[i]=%f, x[i+1]=%f\n",i,xi[i-1],xi[i],xi[i+1]); */

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

  assert(du-du==0.);

  /* printf("D1: i=%i du=%.15f\n", i, du); */
  return du;
}

/* obliczanie drugiej pochodnej */
double D2 ( double * u, double * x, int i, int N )
{
  double ddu;

  if( i > 0 && i < N-1 )
    ddu =
      ((u[i+1]-u[i])/(x[i+1]-x[i])
       -(u[i]-u[i-1])/(x[i]-x[i-1]))
      *2./(x[i+1]-x[i-1]);
  else if (i == 0)
    ddu =(2.*( (u[i]-u[2+i])/(x[i]-x[2+i])
      	      +(-u[1+i]+u[2+i])/(x[1+i]-x[2+i]))
      	    )/(x[i]-x[1+i]);
  else if (i==N-1)
    ddu =  (2.*
	    ( (u[-2+i]-u[i])/(x[-2+i]-x[i])
	      +(-u[-1+i]+u[i])/(x[-1+i]-x[i])
	      )
	    )/(x[-2+i]-x[-1+i]);
  else ddu =  0.;

  assert(ddu-ddu==0.);

  return ddu;

}

/* funkcja definiujaca transformacje Sundmana
   dt/dtau=g(u)=0.01/(du/dx(0,tau))^2 */
double g ( double * y, int N )
{
  double d1,d2;
  d1=fabs(D2(y+1,y+1+N,0,N));
  d2=fabs(D2(y+1,y+1+N,N-1,N));
  d1=d1>d2?d1:d2;

  return
    0.01
    *pow(d1,-2);
}

/* funkcja rozkladu punktow fizycznej siatki (
   M(x)=du/dx+sqrt(d2u/dx2) ) */
void M_calc ( double * ui, double * xi, double * M, int N )
{
  int i;
  double Mtot=0.,du,ddu,x,u;

  for ( i = 1; i < N-1; i++)
    {
      x=xi[i];
      du=D1( ui, xi, i, N );
      ddu=D2( ui, xi, i, N );
      u=ui[i];

      M[i]=fabs( x*du-u )/x/x
	+ sqrt( fabs(
		     x*x*ddu
		     -2.*x*du
		     +2.*u
		     )
		/x/x/x
		);
      Mtot+=(M[i]*(xi[i+1]-xi[i-1])/2.);

      /* printf("M_calc: i=%i, x=%f, M[i]=%.15f\n", */
      /* 	     i, x, M[i]); */
      /* printf("M_calc: M[i]-M[i]=%.15f\n", M[i]-M[i]); */

      /* assert( M[i]-M[i]==0. ); */
      /* assert( M[i] >= 0 ); */
    }

  M[0]=fabs( D2( ui, xi, 0, N )/2. )+sqrt(fabs(D1( ui, xi, 0, N )/3.));

  M[N-1]=fabs( D2( ui, xi, N-1, N ) )+sqrt(fabs((D1( ui, xi, N-1, N ))/3.));

  Mtot+=M[0]*(xi[1]-xi[0])+M[N-1]*(xi[N-1]-xi[N-2]);

  for ( i = 0; i < N; i++ )
    {
      M[i]/=Mtot;
      /* M[i]+=.01*(1.+sin(x[i])); */
    }
  /* printf("----M[0]=%f Mtot=%f\n",M[0],Mtot); */
}
