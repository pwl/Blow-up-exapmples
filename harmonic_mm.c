#include "harmonic.h"

gsl_matrix * D_inv, * C;
double * m, *mtemp;
gsl_vector * fx, * fu, * ftmp;

double k=8.;
extern double mm_A;

int main ( void )
{
  ODE_solver * s;
  int M = 10, K = 0, i;
  int N = 50/* 2*(M+K)+1 */;
  H_DOUBLE T =1.e10;
  H_DOUBLE x0 = 0., x1 = PI, x;
  H_DOUBLE t_error = 1.e-8;
  h_basis_functions * basis = h_basis_finite_difference_5_function_init();
  const gsl_odeiv_step_type * stepper = gsl_odeiv_step_rkf45;
  gsl_matrix * D = gsl_matrix_alloc(N,N);
  gsl_permutation * p = gsl_permutation_alloc(N);
  FILE * file;

  H_DOUBLE A,A1,B1;

  /* Alokacja pamięci i inicjalizacja oraz odwrócenie macierzy przy
     użyciu GSL */
  m = malloc( N*sizeof(double) );
  mtemp = malloc( N*sizeof(double) );
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
  s = ODE_solver_init ( 2*N+1, /*rk=*/ 1, T, x0, x1, t_error, basis, ODE_set, NULL, stepper );
  /* inicjalizacja modułów obrazujących przebieg symulacji, pierwszy
     argument to odstęp (mierzony czasem obliczeniowym) w jakim mają
     być wywoływane kolejne moduły */
  /* modul do wizualizacji wykresu fcji w czasie rzeczywistym */
  ODE_modules_add ( s, ODE_module_plot_init( 10. ) );
  /* modul do drukowania w konsoli czasu symulacji */
  ODE_modules_add ( s, ODE_module_print_time_init ( .0 ) );
  /* modul do wpisywania do pliku log/info_1/log001.dat szeregu
     informacji dot. funkcji, w kolejnosci sa to:
     tau, t, u[1], x[1], du(0,tau)/dx, g, *dtau, 0. */
  ODE_modules_add ( s, ODE_module_info_1_init( .01, N ) );
  /* modul wpisywania profili fcji do katalogu log/snapshot */
  /* ODE_modules_add ( s, ODE_module_snapshot_init( 1. )); */
  ODE_modules_add ( s, ODE_module_bisection_3_init( .001 ));
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

  /* x=x0; */

  /* s->state->f[1+N]=0.; */
  /* s->state->f[1]=0.; */

  /* x=x0+(x1-x0)/(N-1.); */

  /* asym: N=300, x0=0, x1=PI/2, T=5.*/
  /* A=1.24568469439604978533; */

  /* asym: N=400, x0=0, x1=PI/2, T=5.*/
  /* A=1.24571310318894035163-1.1e-14; */

  /* sym: N=400, x0=0, x1=PI, T=5.*/
  /* A=2.10669393489537526420; */

  /* mm_A=1.55129424574719898263; */
  /* mm_A=PI; */

  for ( i = 0; i < N; i++ ) {
    x=i*(x1-x0)/(N-1);
    s->state->f[i+1+N]=x;
  }

  mm_setup_mesh( s->state->f+1+N, N );

  for ( i = 0; i < N; i++ ) {
    x=s->state->f[i+1+N];
    s->state->f[i+1]=mm_u(x);
  }

  s->state->f[0]=0.;

  /* ODE_solve ( s ); */

  bisec(0.,PI,10.e-15,0.,
  	bisection_wrapper,(void*)s);

  /* file = fopen ( "test.dat", "w" ); */
  /* for ( i = 0; i < N; i++ ) */
  /*   fprintf(file, "%i %.15G %.15G\n", i, s->state->f[i+1+N], s->state->f[i+1]); */

  /* fclose( file ); */

  /* ponizsza funkcja pcha wartosci poczatkowe w czasie i uruchamia
     poszczegolne moduly */

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
  H_DOUBLE u,x,du,ddu,Mxi,de,epsilon,gt,du0;
  H_DOUBLE x1 = s->params->basis->params->x1;
  H_DOUBLE dt = *(s->state->dt);

  /* definicje zmiennych pomocniczych */
  de	  = 1./(N-1);
  M_calc( ui, xi, m, N );
  /* epsilon = min(max(1.e-3,m[0]),1.e-1); */
  /* gt = .01*2*pow(fabs(D1(ui,xi,0,N)/2.73875312588499),-2); /\* gt=alpha*du/dx/(d2u/dxdt)|x=0 *\/ */
  /* epsilon = sqrt(1.e2*gt);/\* min(max(1.e-5,1.e2*gt),1.e-1); *\/ */
  /* gt = g( y, N ); */
  /* gt=1.; */
  /* assert(!isnan(gt)); */
  /* assert(!isnan(y[0])); */

  /* if( dt*gt < 1.e-15) */
  /*   { */
  /*     printf("STOP: gt*dt = %.5G < 1.e-15\n", gt*dt); */
  /*     s->state->status = SOLVER_STATUS_STOP; */
  /*     return; */
  /*   } */

  for ( i = 1; i < N-1; i++) {
    u=ui[i];
    x=xi[i];
    /* obliczenie pochodnych w punkcie "i" */
    du=D1(ui,xi,i,N);
    ddu=D2(ui,xi,i,N);

    /* prawa strona rownania macierzowego */
    Mxi = ((m[i+1]+m[i])*(xi[i+1]-xi[i])-(m[i]+m[i-1])*(xi[i]-xi[i-1]))/2./de/de;
    assert(!isnan(Mxi));
    /* Mxi=0.; */

    gsl_vector_set(fu, i,
		   (ddu+(k-1.)/x*du-(k-1.)/2.*sin(2.*u)/x/x));
    gsl_vector_set(ftmp, i,
		   Mxi);
    gsl_matrix_set(C, i, i, -du);
  }

  for ( i = 1; i < N-1; i++) {
    f[i+1] = gsl_vector_get(fu,i); /* tymczasowe miejsce dla du/dt */
  }

  /* printf ("%.5G\n",.01*dt*D1(ui,xi,0,N)/D1(f+1,xi,0,N)); */
  gt = .01*(fabs(D1(ui,xi,0,N))+1.)/(fabs(D1(f+1,xi,0,N))+1.); /* gt=alpha*du/dx/(d2u/dxdt)|x=0 */
  epsilon = sqrt(1.e2*gt);/* min(max(1.e-5,1.e2*gt),1.e-1); */

  if( gt*dt < 1.e-15)
    {
      printf("STOP: gt*dt = %.5G < 1.e-15\n", gt*dt);
      s->state->status = SOLVER_STATUS_STOP;
      return;
    }

  /* przepisanie wynikow do tablicy pochodnej czasowej */
  gsl_blas_dsymv (CblasUpper, -1./epsilon, D_inv, ftmp, 0., fx); /* D = -d2/de2 */
  gsl_blas_dgemv (CblasNoTrans, -1., C, fx, 1., fu); /* C = -du/dx */

  for ( i = 1; i < N-1; i++) {
    f[i+1]   = gt*gsl_vector_get(fu,i);
    f[i+N+1] = gt*gsl_vector_get(fx,i);
  }

  /* warunki brzegowe */
  f[1]=0.;
  f[N]=0.;
  f[N+1]=0.;
  f[2*N]=0.;

  f[0]=gt;
}

double D1 ( double * u, double * x, int i, int N )
{
  double du;

  /* for ( i = 0; i < N; i++ ) */
  /*   printf("%i %.15f %.15f\n", i, x[i], u[i]); */

  if( i > 0 && i < N-1 )
    return (u[i]*(-1 - (x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i]) +
           x[-2 + i]*(x[-1 + i] - 2*x[i] + x[1 + i]))*(x[i] - x[2 + i])*pow(x[-2 + i] - x[i],-1)*
         pow(x[-1 + i] - x[i],-1)*pow(x[i] - x[1 + i],-1)) +
     u[-2 + i]*(x[-1 + i] - x[i])*(x[i] - x[1 + i])*pow(x[-2 + i] - x[-1 + i],-1)*
      pow(x[-2 + i] - x[i],-1)*pow(x[-2 + i] - x[1 + i],-1)*pow(x[-2 + i] - x[2 + i],-1)*
      pow(x[i] - x[2 + i],2) - u[-1 + i]*(x[-2 + i] - x[i])*(x[i] - x[1 + i])*
      pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-1 + i] - x[i],-1)*pow(x[-1 + i] - x[1 + i],-1)*
      pow(x[-1 + i] - x[2 + i],-1)*pow(x[i] - x[2 + i],2) +
     u[1 + i]*(-x[-2 + i] + x[i])*(-x[-1 + i] + x[i])*pow(x[-2 + i] - x[1 + i],-1)*
      pow(-x[-1 + i] + x[1 + i],-1)*pow(-x[i] + x[1 + i],-1)*pow(x[i] - x[2 + i],2)*
      pow(x[1 + i] - x[2 + i],-1) + u[2 + i]*(x[-2 + i] - x[i])*(-x[-1 + i] + x[i])*(x[i] - x[1 + i])*
      pow(x[1 + i] - x[2 + i],-1)*pow(-x[-2 + i] + x[2 + i],-1)*pow(-x[-1 + i] + x[2 + i],-1))*
      pow(-x[i] + x[2 + i],-1);

  else if( i == 0 )
    return pow(-x[i] + x[4 + i],-1)*(u[i]*(-1 + (-x[i] + x[4 + i])*
         (-1 + (-2*x[i] + x[1 + i] + x[2 + i])*(-x[i] + x[3 + i])*pow(x[i] - x[1 + i],-1)*
            pow(-x[i] + x[2 + i],-1))*pow(-x[i] + x[3 + i],-1)) +
     u[1 + i]*(x[i] - x[2 + i])*(x[i] - x[3 + i])*pow(x[i] - x[1 + i],-1)*pow(x[1 + i] - x[2 + i],-1)*
      pow(x[1 + i] - x[3 + i],-1)*pow(x[i] - x[4 + i],2)*pow(x[1 + i] - x[4 + i],-1) +
     u[3 + i]*(x[i] - x[1 + i])*(x[i] - x[2 + i])*pow(x[2 + i] - x[3 + i],-1)*pow(-x[i] + x[3 + i],-1)*
      pow(-x[1 + i] + x[3 + i],-1)*pow(x[i] - x[4 + i],2)*pow(x[3 + i] - x[4 + i],-1) +
     u[2 + i]*(x[i] - x[1 + i])*(-x[i] + x[3 + i])*pow(-x[i] + x[2 + i],-1)*pow(-x[1 + i] + x[2 + i],-1)*
      pow(-x[2 + i] + x[3 + i],-1)*pow(x[i] - x[4 + i],2)*pow(-x[2 + i] + x[4 + i],-1) +
     u[4 + i]*(x[i] - x[1 + i])*(x[i] - x[2 + i])*(x[i] - x[3 + i])*pow(-x[1 + i] + x[4 + i],-1)*
				    pow(-x[2 + i] + x[4 + i],-1)*pow(-x[3 + i] + x[4 + i],-1));
  else if ( i == 1 )
    return pow(-x[i] + x[3 + i],-1)*(u[i]*(-1 + (-x[i] + x[3 + i])*
         (-1 - (x[-1 + i] - 2*x[i] + x[1 + i])*(x[i] - x[2 + i])*pow(x[-1 + i] - x[i],-1)*
            pow(x[i] - x[1 + i],-1))*pow(-x[i] + x[2 + i],-1)) -
     u[-1 + i]*(x[i] - x[1 + i])*(x[i] - x[2 + i])*pow(x[-1 + i] - x[i],-1)*pow(x[-1 + i] - x[1 + i],-1)*
      pow(x[-1 + i] - x[2 + i],-1)*pow(x[-1 + i] - x[3 + i],-1)*pow(x[i] - x[3 + i],2) +
     u[1 + i]*(x[-1 + i] - x[i])*(x[i] - x[2 + i])*pow(x[-1 + i] - x[1 + i],-1)*pow(x[i] - x[1 + i],-1)*
      pow(x[1 + i] - x[2 + i],-1)*pow(x[i] - x[3 + i],2)*pow(x[1 + i] - x[3 + i],-1) +
     u[2 + i]*(-x[-1 + i] + x[i])*(x[i] - x[1 + i])*pow(x[-1 + i] - x[2 + i],-1)*
      pow(-x[i] + x[2 + i],-1)*pow(-x[1 + i] + x[2 + i],-1)*pow(x[i] - x[3 + i],2)*
      pow(x[2 + i] - x[3 + i],-1) + u[3 + i]*(x[-1 + i] - x[i])*(x[i] - x[1 + i])*(x[i] - x[2 + i])*
				pow(x[2 + i] - x[3 + i],-1)*pow(-x[-1 + i] + x[3 + i],-1)*pow(-x[1 + i] + x[3 + i],-1));
  else if ( i == N-2 )
    return (-(u[i]*(x[-2 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i])) +
          x[-3 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i]) +
             x[-2 + i]*(x[-1 + i] - 2*x[i] + x[1 + i])) +
          x[i]*(x[-1 + i]*(3*x[i] - 2*x[1 + i]) + x[i]*(-4*x[i] + 3*x[1 + i])))*pow(x[-2 + i] - x[i],-1)*
        pow(-x[-3 + i] + x[i],-1)*pow(-x[-1 + i] + x[i],-1)) +
     u[-3 + i]*(x[-2 + i] - x[i])*(-x[-1 + i] + x[i])*pow(x[-3 + i] - x[-2 + i],-1)*
      pow(x[-3 + i] - x[-1 + i],-1)*pow(x[-3 + i] - x[i],-1)*pow(x[-3 + i] - x[1 + i],-1)*
      pow(x[i] - x[1 + i],2) + u[-2 + i]*(x[-3 + i] - x[i])*(x[-1 + i] - x[i])*
      pow(x[-3 + i] - x[-2 + i],-1)*pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-2 + i] - x[i],-1)*
      pow(x[-2 + i] - x[1 + i],-1)*pow(x[i] - x[1 + i],2) +
     u[-1 + i]*(x[-3 + i] - x[i])*(-x[-2 + i] + x[i])*pow(x[-3 + i] - x[-1 + i],-1)*
      pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-1 + i] - x[i],-1)*pow(x[-1 + i] - x[1 + i],-1)*
      pow(x[i] - x[1 + i],2) + u[1 + i]*(x[-3 + i] - x[i])*(-x[-2 + i] + x[i])*(-x[-1 + i] + x[i])*
      pow(x[-1 + i] - x[1 + i],-1)*pow(-x[-3 + i] + x[1 + i],-1)*pow(-x[-2 + i] + x[1 + i],-1))*
	pow(-x[i] + x[1 + i],-1);
  else if( i == N-1 )
    return pow(x[-2 + i] - x[i],-1)*pow(-x[-4 + i] + x[i],-1)*
   (u[-2 + i]*(x[-3 + i] - x[i])*(x[-1 + i] - x[i])*pow(x[-4 + i] - x[-2 + i],-1)*
      pow(x[-3 + i] - x[-2 + i],-1)*pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-4 + i] - x[i],2) +
     u[-4 + i]*(x[-3 + i] - x[i])*(x[-1 + i] - x[i])*pow(x[-4 + i] - x[-3 + i],-1)*
      pow(x[-4 + i] - x[-2 + i],-1)*pow(x[-4 + i] - x[-1 + i],-1)*pow(x[-2 + i] - x[i],2) +
     u[-3 + i]*(-x[-1 + i] + x[i])*pow(x[-4 + i] - x[-3 + i],-1)*pow(x[-3 + i] - x[-2 + i],-1)*
      pow(x[-3 + i] - x[-1 + i],-1)*pow(x[-4 + i] - x[i],2)*pow(x[-3 + i] - x[i],-1)*
      pow(x[-2 + i] - x[i],2) + u[-1 + i]*(-x[-3 + i] + x[i])*pow(x[-3 + i] - x[-1 + i],-1)*
      pow(-x[-4 + i] + x[-1 + i],-1)*pow(-x[-2 + i] + x[-1 + i],-1)*pow(x[-4 + i] - x[i],2)*
      pow(x[-2 + i] - x[i],2)*pow(x[-1 + i] - x[i],-1) +
     u[i]*(x[i]*((3*x[-1 + i] - 4*x[i])*x[i] + x[-2 + i]*(-2*x[-1 + i] + 3*x[i])) +
        x[-3 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) + x[i]*(-2*x[-1 + i] + 3*x[i])) +
        x[-4 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) + x[-3 + i]*(x[-2 + i] + x[-1 + i] - 2*x[i]) +
		   x[i]*(-2*x[-1 + i] + 3*x[i])))*pow(-x[-3 + i] + x[i],-1)*pow(-x[-1 + i] + x[i],-1));
  return 0.;
}

/* obliczanie drugiej pochodnej */
double D2 ( double * u, double * x, int i, int N )
{
  if( i > 1 && i < N-2 )
    return 2*(u[-2 + i]*pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-2 + i] - x[i],-1)*
      (x[1 + i]*x[2 + i] - 2*x[i]*(x[1 + i] + x[2 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i] + x[2 + i]) +
        3*pow(x[i],2))*pow(x[-2 + i] - x[1 + i],-1)*pow(x[-2 + i] - x[2 + i],-1) +
     u[-1 + i]*pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-1 + i] - x[i],-1)*
      (x[-2 + i]*(2*x[i] - x[1 + i] - x[2 + i]) - x[1 + i]*x[2 + i] + 2*x[i]*(x[1 + i] + x[2 + i]) -
        3*pow(x[i],2))*pow(x[-1 + i] - x[1 + i],-1)*pow(x[-1 + i] - x[2 + i],-1) +
     u[i]*pow(x[-2 + i] - x[i],-1)*pow(x[-1 + i] - x[i],-1)*
      (-3*x[i]*x[1 + i] - 3*x[i]*x[2 + i] + x[1 + i]*x[2 + i] +
        x[-1 + i]*(-3*x[i] + x[1 + i] + x[2 + i]) +
        x[-2 + i]*(x[-1 + i] - 3*x[i] + x[1 + i] + x[2 + i]) + 6*pow(x[i],2))*pow(x[i] - x[1 + i],-1)*
      pow(x[i] - x[2 + i],-1) - u[1 + i]*(x[i]*(3*x[i] - 2*x[2 + i]) + x[-1 + i]*(-2*x[i] + x[2 + i]) +
        x[-2 + i]*(x[-1 + i] - 2*x[i] + x[2 + i]))*pow(x[-1 + i] - x[1 + i],-1)*
      pow(-x[-2 + i] + x[1 + i],-1)*pow(-x[i] + x[1 + i],-1)*pow(x[1 + i] - x[2 + i],-1) -
     u[2 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i]) +
        x[-2 + i]*(x[-1 + i] - 2*x[i] + x[1 + i]))*pow(x[-1 + i] - x[2 + i],-1)*
	      pow(-x[-2 + i] + x[2 + i],-1)*pow(-x[i] + x[2 + i],-1)*pow(-x[1 + i] + x[2 + i],-1));
  else if (i == 0)
    return 2*u[i]*(x[2 + i]*x[3 + i] + x[2 + i]*x[4 + i] + x[3 + i]*x[4 + i] +
      x[1 + i]*(x[2 + i] + x[3 + i] + x[4 + i]) - 3*x[i]*(x[1 + i] + x[2 + i] + x[3 + i] + x[4 + i]) +
      6*pow(x[i],2))*pow(x[i] - x[1 + i],-1)*pow(x[i] - x[2 + i],-1)*pow(x[i] - x[3 + i],-1)*
    pow(x[i] - x[4 + i],-1) - 2*u[1 + i]*(x[3 + i]*x[4 + i] + x[2 + i]*(x[3 + i] + x[4 + i]) -
      2*x[i]*(x[2 + i] + x[3 + i] + x[4 + i]) + 3*pow(x[i],2))*pow(x[i] - x[1 + i],-1)*
    pow(x[1 + i] - x[2 + i],-1)*pow(x[1 + i] - x[3 + i],-1)*pow(x[1 + i] - x[4 + i],-1) +
   2*u[2 + i]*(x[3 + i]*x[4 + i] + x[1 + i]*(x[3 + i] + x[4 + i]) -
      2*x[i]*(x[1 + i] + x[3 + i] + x[4 + i]) + 3*pow(x[i],2))*pow(x[i] - x[2 + i],-1)*
    pow(x[1 + i] - x[2 + i],-1)*pow(x[2 + i] - x[3 + i],-1)*pow(x[2 + i] - x[4 + i],-1) -
   2*u[3 + i]*(x[2 + i]*x[4 + i] + x[1 + i]*(x[2 + i] + x[4 + i]) -
      2*x[i]*(x[1 + i] + x[2 + i] + x[4 + i]) + 3*pow(x[i],2))*pow(x[1 + i] - x[3 + i],-1)*
    pow(-x[i] + x[3 + i],-1)*pow(-x[2 + i] + x[3 + i],-1)*pow(x[3 + i] - x[4 + i],-1) -
   2*u[4 + i]*(x[2 + i]*x[3 + i] + x[1 + i]*(x[2 + i] + x[3 + i]) -
      2*x[i]*(x[1 + i] + x[2 + i] + x[3 + i]) + 3*pow(x[i],2))*pow(x[1 + i] - x[4 + i],-1)*
      pow(-x[i] + x[4 + i],-1)*pow(-x[2 + i] + x[4 + i],-1)*pow(-x[3 + i] + x[4 + i],-1);
  else if (i==1)
    return  2*(u[-1 + i]*pow(x[-1 + i] - x[i],-1)*(x[2 + i]*x[3 + i] + x[1 + i]*(x[2 + i] + x[3 + i]) -
        2*x[i]*(x[1 + i] + x[2 + i] + x[3 + i]) + 3*pow(x[i],2))*pow(x[-1 + i] - x[1 + i],-1)*
      pow(x[-1 + i] - x[2 + i],-1)*pow(x[-1 + i] - x[3 + i],-1) +
     u[i]*pow(x[-1 + i] - x[i],-1)*(-(x[1 + i]*x[2 + i]) +
        x[-1 + i]*(3*x[i] - x[1 + i] - x[2 + i] - x[3 + i]) - x[1 + i]*x[3 + i] - x[2 + i]*x[3 + i] +
        3*x[i]*(x[1 + i] + x[2 + i] + x[3 + i]) - 6*pow(x[i],2))*pow(x[i] - x[1 + i],-1)*
      pow(x[i] - x[2 + i],-1)*pow(x[i] - x[3 + i],-1) +
     u[1 + i]*(x[2 + i]*x[3 + i] - 2*x[i]*(x[2 + i] + x[3 + i]) +
        x[-1 + i]*(-2*x[i] + x[2 + i] + x[3 + i]) + 3*pow(x[i],2))*pow(x[-1 + i] - x[1 + i],-1)*
      pow(x[i] - x[1 + i],-1)*pow(x[1 + i] - x[2 + i],-1)*pow(x[1 + i] - x[3 + i],-1) -
     u[2 + i]*(x[1 + i]*x[3 + i] - 2*x[i]*(x[1 + i] + x[3 + i]) +
        x[-1 + i]*(-2*x[i] + x[1 + i] + x[3 + i]) + 3*pow(x[i],2))*pow(x[i] - x[2 + i],-1)*
      pow(-x[-1 + i] + x[2 + i],-1)*pow(-x[1 + i] + x[2 + i],-1)*pow(x[2 + i] - x[3 + i],-1) -
     u[3 + i]*(x[1 + i]*x[2 + i] - 2*x[i]*(x[1 + i] + x[2 + i]) +
        x[-1 + i]*(-2*x[i] + x[1 + i] + x[2 + i]) + 3*pow(x[i],2))*pow(x[1 + i] - x[3 + i],-1)*
	       pow(-x[-1 + i] + x[3 + i],-1)*pow(-x[i] + x[3 + i],-1)*pow(-x[2 + i] + x[3 + i],-1));
  else if (i==N-2)
    return 2*u[-3 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i]) +
      x[-2 + i]*(x[-1 + i] - 2*x[i] + x[1 + i]))*pow(x[-3 + i] - x[-2 + i],-1)*
    pow(x[-3 + i] - x[-1 + i],-1)*pow(x[-3 + i] - x[i],-1)*pow(x[-3 + i] - x[1 + i],-1) -
   2*u[-2 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i]) +
      x[-3 + i]*(x[-1 + i] - 2*x[i] + x[1 + i]))*pow(x[-3 + i] - x[-2 + i],-1)*
    pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-2 + i] - x[i],-1)*pow(x[-2 + i] - x[1 + i],-1) +
   2*u[-1 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-2 + i]*(-2*x[i] + x[1 + i]) +
      x[-3 + i]*(x[-2 + i] - 2*x[i] + x[1 + i]))*pow(x[-3 + i] - x[-1 + i],-1)*
    pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-1 + i] - x[i],-1)*pow(x[-1 + i] - x[1 + i],-1) -
   2*u[i]*pow(x[-2 + i] - x[i],-1)*(-3*x[-1 + i]*x[i] + x[-1 + i]*x[1 + i] - 3*x[i]*x[1 + i] +
      x[-2 + i]*(x[-1 + i] - 3*x[i] + x[1 + i]) +
      x[-3 + i]*(x[-2 + i] + x[-1 + i] - 3*x[i] + x[1 + i]) + 6*pow(x[i],2))*pow(-x[-3 + i] + x[i],-1)*
    pow(-x[-1 + i] + x[i],-1)*pow(x[i] - x[1 + i],-1) -
   2*u[1 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) + x[-3 + i]*(x[-2 + i] + x[-1 + i] - 2*x[i]) +
      x[i]*(-2*x[-1 + i] + 3*x[i]))*pow(x[-2 + i] - x[1 + i],-1)*pow(-x[-3 + i] + x[1 + i],-1)*
      pow(-x[-1 + i] + x[1 + i],-1)*pow(-x[i] + x[1 + i],-1);

  else if (i==N-1)
    return 2*u[-4 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) + x[-3 + i]*(x[-2 + i] + x[-1 + i] - 2*x[i]) +
      x[i]*(-2*x[-1 + i] + 3*x[i]))*pow(x[-4 + i] - x[-3 + i],-1)*pow(x[-4 + i] - x[-2 + i],-1)*
    pow(x[-4 + i] - x[-1 + i],-1)*pow(x[-4 + i] - x[i],-1) -
   2*u[-3 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) + x[-4 + i]*(x[-2 + i] + x[-1 + i] - 2*x[i]) +
      x[i]*(-2*x[-1 + i] + 3*x[i]))*pow(x[-4 + i] - x[-3 + i],-1)*pow(x[-3 + i] - x[-2 + i],-1)*
    pow(x[-3 + i] - x[-1 + i],-1)*pow(x[-3 + i] - x[i],-1) +
   2*u[-2 + i]*(x[-3 + i]*(x[-1 + i] - 2*x[i]) + x[-4 + i]*(x[-3 + i] + x[-1 + i] - 2*x[i]) +
      x[i]*(-2*x[-1 + i] + 3*x[i]))*pow(x[-4 + i] - x[-2 + i],-1)*pow(x[-3 + i] - x[-2 + i],-1)*
    pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-2 + i] - x[i],-1) -
   2*u[-1 + i]*(x[-3 + i]*(x[-2 + i] - 2*x[i]) + x[-4 + i]*(x[-3 + i] + x[-2 + i] - 2*x[i]) +
      x[i]*(-2*x[-2 + i] + 3*x[i]))*pow(x[-3 + i] - x[-1 + i],-1)*pow(-x[-4 + i] + x[-1 + i],-1)*
    pow(-x[-2 + i] + x[-1 + i],-1)*pow(x[-1 + i] - x[i],-1) -
   2*u[i]*pow(x[-3 + i] - x[i],-1)*(x[-2 + i]*x[-1 + i] + x[-3 + i]*(x[-2 + i] + x[-1 + i] - 3*x[i]) +
      x[-4 + i]*(x[-3 + i] + x[-2 + i] + x[-1 + i] - 3*x[i]) - 3*x[-2 + i]*x[i] - 3*x[-1 + i]*x[i] +
				    6*pow(x[i],2))*pow(-x[-4 + i] + x[i],-1)*pow(-x[-2 + i] + x[i],-1)*pow(-x[-1 + i] + x[i],-1);
  else return 0.;
}

/* funkcja definiujaca transformacje Sundmana
   dt/dtau=g(u)=0.01/(du/dx(0,tau))^2 */
double g ( double * y, int N )
{
  /* return 0.01*pow(fabs(D1(y+1,y+1+N,0,N))+fabs(D1(y+1,y+1+N,N-1,N)),-2); */
  H_DOUBLE * ui = y + 1;
  H_DOUBLE * xi = y + 1 + N;
  double du=D1(ui,xi,1,N),ddu=D2(ui,xi,1,N);
  double x=xi[1];
  double u=ui[1];
  double ut=(ddu+((k-1.)/x-x/2.)*du-(k-1.)/2.*sin(2.*u)/x/x);
  /* printf("du=%f, ut=%f, x=%f\n",du,ut,x); */
  return .01*(fabs(x*du/ut));
}

/* funkcja rozkladu punktow fizycznej siatki (
   M(x)=du/dx+sqrt(d2u/dx2) ) */
void M_calc ( double * u, double * x, double * M, int N )
{
  int i;
  double Mtot=0.;

  for ( i = 0; i < N; i++)
    {
      M[i]=fabs( D1( u, x, i, N ) ) + sqrt(fabs( D2( u, x, i, N ) ));
      assert( M[i]-M[i]==0. );
      assert( M[i] >= 0 );
    }
  /* M_smoothen ( M, mtemp, N, 10., 3 ); */
}

double bisection_wrapper(double A, void * p)
{
  ODE_solver * s = (ODE_solver*)p;
  ODE_module * m = ODE_modules_get_by_name(s,"bisection");
  double x0 = s->params->basis->params->x0;
  double x1 = s->params->basis->params->x1;
  double x,A1,B1;
  int i, N = (s->params->Nx-1)/2;
  FILE * file;

  s->state->f[0]=0.;
  /* s->state->t[0]=0.; */
  s->state->f[1+N]=0.;
  s->state->f[1]=0.;

  mm_A=A;

  for ( i = 0; i < N; i++ ) {
    x=i*(x1-x0)/(N-1);
    s->state->f[i+1+N]=x;
  }

  mm_setup_mesh( s->state->f+1+N, N );

  for ( i = 0; i < N; i++ ) {
    x=s->state->f[i+1+N];
    s->state->f[i+1]=mm_u(x);
  }
  *(s->state->t) = 0.;
  *(s->state->dt) = 1.e-10;
  s->state->f[0] = 0.;
  /* for ( i = 0; i < N; i++ ) { */
  /*   x=i*(x1-x0)/(N-1); */
  /*   s->state->f[i+1+N]=x; */
  /*   s->state->f[i+1]=x+A*sin(2.*x); */
  /*   /\* s->state->f[i+1]=A*sin(x); *\/ */
  /* } */

  file = fopen ( "test.dat", "w" );
  for ( i = 0; i < N; i++ )
    fprintf(file, "%i %.15G %.15G\n", i, s->state->f[i+1+N], s->state->f[i+1]);

  fclose( file );

  s->state->status = SOLVER_STATUS_OK;

  ODE_solve(s);

  printf("T_max = %.15f\n",s->state->t[0]);

  return ((bisection_3_module_data*)m->data)->result;
}


double
bisec(double A0,
      double A1,
      double e,
      double val,
      double (*fevol)(double, void *),
      void * param)
{
  double f,f0,f1;
  int i=1;
  double B;

  /* swap the values if they are not sorted */
  if ( A0 > A1 )
    {
      B = A0;
      A0 = A1;
      A1 = B;
    }

  f0 = fevol( A0, param ) - val;
  f1 = fevol( A1, param ) - val;

  while( 2.*(A1-A0)/fabs(A1+A0) > e) /* relative error measure */
    {
      printf( "%03i, A=%.20f, delta/A=%.1E\n",
	      i++, .5*(A0+A1), 2.*(A1-A0)/(A0+A1) );

      f = fevol(.5*(A0+A1), param ) - val;

      if( f*f0 > 0 )
	A0=.5*(A0+A1);
      else
	A1=.5*(A0+A1);
    }
  printf("\n");

  return .5*(A0+A1);
}

void M_smoothen ( double * M, double * Mtemp, int N, double gamma, int ip )
{
  double sumdenom, sumnom;
  int i,j;

  sumdenom = 0.;
  for ( j = -ip; j <= ip; j++ )
    sumdenom += pow(gamma/(1+gamma),abs(j));

  for ( i = ip; i < N - ip; i++ )
    {
      sumnom = 0.;
      for ( j = -ip; j <= ip; j++ )
	sumnom += pow( M[i+j], 2 ) * pow( gamma/(1+gamma), abs(j) );
      Mtemp[i] = sqrt( sumnom / sumdenom );
    }

  for ( i = 0; i < ip; i++ )
    {
      sumnom = 0.;
      for ( j = 0; j <= i+ip; j++ )
	sumnom += pow( M[j], 2 ) * pow( gamma/(1+gamma), abs(i-j) );
      for ( j = i-ip; j < 0; j++ ) /* assuming M[i]=M[0] for i<0 */
	sumnom += pow( M[0], 2 ) * pow( gamma/(1+gamma), abs(i-j) );

      Mtemp[i] = sqrt( sumnom / sumdenom );
    }

  for ( i = N-ip; i < N; i++ )
    {
      sumnom = 0.;
      for ( j = i-ip; j < N; j++ )
	sumnom += pow( M[j], 2 ) * pow( gamma/(1+gamma), abs(i-j) );
      for ( j = N; j <= i+ip; j++ ) /* assuming M[i]=M[N-1] for i>N-1 */
	sumnom += pow( M[N-1], 2 ) * pow( gamma/(1+gamma), abs(i-j) );

      Mtemp[i] = sqrt( sumnom / sumdenom );
    }


  /* write the somoothened function to M */
  for ( i = 0; i < N; i++ )
    {
      M[i]=Mtemp[i];
    }
}
