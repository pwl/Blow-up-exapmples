#include "harmonic.h"

gsl_matrix * D_inv, * C;
double * m, * mtemp;
gsl_vector * fx, * fu, * ftmp;

double k=3.;
extern double mm_A;
int hold_mesh=0;


int main ( void )
{
  ODE_solver * s;
  int M = 10, K = 0, i;
  int N = 150/* 2*(M+K)+1 */;
  H_DOUBLE T =1.e11;
  H_DOUBLE x0 = 0., x1 = PI, x, du, ddu;
  H_DOUBLE t_error = 1.e-11;
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
      gsl_vector_set(ftmp, i, 0.);
    }

  gsl_matrix_set(D, N-1, N-1, 1.);
  gsl_matrix_set(D, 0, 0, 1.);
  gsl_vector_set(ftmp,0,0.);

  gsl_linalg_LU_decomp(D, p, &i );
  gsl_linalg_LU_invert(D, p, D_inv);

  /* inicjalizacja struktury przechowującej informacje dot. dymulacji */
  s = ODE_solver_init ( 2*N+1, /*rk=*/ 1, T, x0, x1, t_error, basis, ODE_set, NULL, stepper );
  /* inicjalizacja modułów obrazujących przebieg symulacji, pierwszy
     argument to odstęp (mierzony czasem obliczeniowym) w jakim mają
     być wywoływane kolejne moduły */
  /* modul do wizualizacji wykresu fcji w czasie rzeczywistym */
  ODE_modules_add ( s, ODE_module_plot_init( 10. ) );
  ODE_modules_add ( s, ODE_module_plot_x_init( 10. ) );
  /* modul do drukowania w konsoli czasu symulacji */
  ODE_modules_add ( s, ODE_module_print_time_init ( .0 ) );
  /* modul do wpisywania do pliku log/info_1/log001.dat szeregu
     informacji dot. funkcji, w kolejnosci sa to:
     tau, t, u[1], x[1], du(0,tau)/dx, g, *dtau, 0. */
  ODE_modules_add ( s, ODE_module_info_1_init( .01, N ) );
  /* modul wpisywania profili fcji do katalogu log/snapshot */
  ODE_modules_add ( s, ODE_module_snapshot_init( .1 ));
  /* ODE_modules_add ( s, ODE_module_bisection_3_init( .001 )); */
  /* ODE_modules_add ( s, ODE_module_movie_maker_init( 0.) ); */

  /* inicjalizacja danych poczatkowych */
  mm_A=1.;

  for ( i = 0; i < N; i++ ) {
    x=i*(x1-x0)/(N-1);
    s->state->f[i+1+N]=x;
  }

  /* mm_setup_mesh( s->state->f+1+N, N ); */

  for ( i = 0; i < N; i++ ) {
    x=s->state->f[i+1+N];
    s->state->f[i+1]=mm_u(x)*x;
  }

  s->state->f[0]=0.;

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
  H_DOUBLE u,x,du,ddu,Mxi,de,epsilon,gt,du0;
  H_DOUBLE x1 = s->params->basis->params->x1;
  H_DOUBLE dt = *(s->state->dt);

  de	  = 1./(N-1);

  for ( i = 1; i < N; i++)
    s->params->basis->ai_temp[i]=ui[i]/xi[i];

  s->params->basis->ai_temp[0]=D1(ui,xi,0,N);

  M_calc( s->params->basis->ai_temp, xi, m, N );

  for ( i = 1; i < N-1; i++) {
    u=ui[i];
    x=xi[i];

    /* obliczenie pochodnych w punkcie "i" */
    du=D1(ui,xi,i,N);
    ddu=D2(ui,xi,i,N);

    /* prawa strona rownania macierzowego */
    Mxi = ((m[i+1]+m[i])*(xi[i+1]-xi[i])-(m[i]+m[i-1])*(xi[i]-xi[i-1]))/2./de/de;
    /* assert(!isnan(Mxi)); */
    /* Mxi=0.; */

    gsl_vector_set(fu, i,
		   (ddu-sin(2.*u/x)/x));
    gsl_vector_set(ftmp, i,
		   Mxi);
    gsl_matrix_set(C, i, i, -du);
  }

  for ( i = 1; i < N-1; i++)
    f[i+1] = gsl_vector_get(fu,i); /* tymczasowe miejsce dla du/dt */

  gt = .01*(fabs(D2(ui,xi,0,N))+1.)/fabs(D2(f+1,xi,0,N));	/* gt=alpha*du/dx/(d2u/dxdt)|x=0 */
  /* gt = .01*fabs(.5/pow(D2(ui,xi,0,N),2));	/\* gt=alpha*du/dx/(d2u/dxdt)|x=0 *\/ */
  /* gt = min(1.e1,gt); */
  /* epsilon = 1.e-3; */
  /* gt=.0002; */


  gt+=1.e-12;


  epsilon = 1.e2*sqrt(gt)+1.e-1;

  /* if( gt < 1.e-14 ) */
  /*   { */
  /*     for ( i = 1; i < N-1; i++) */
  /* 	gsl_vector_set(ftmp,i,0.); */
  /*   } */

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

/* funkcja rozkladu punktow fizycznej siatki (
   M(x)=du/dx+sqrt(d2u/dx2) ) */
void M_calc ( double * u, double * x, double * M, int N )
{
  int i;
  double Mtot=0.,du,ddu;

  for ( i = 0; i < N; i++)
    {
      du=D1( u, x, i, N );
      ddu=D2( u, x, i, N );
      M[i]=fabs(du) + sqrt(fabs(ddu));
      /* assert( !isnan(M[i]) ); */
      /* assert( M[i] >= 0 ); */
    }
  M_smoothen ( M, mtemp, N, 5., 5 );
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
