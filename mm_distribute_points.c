#include "mm_distribute_points.h"


double mm_A;


double mm_u
( double x )
{
  return mm_A*sin(x)+x/PI*PI/10.;
  /* return (exp (pow (x, 2))*sin (x))/300.; */
}

double mm_du
( double x )
{
  return mm_A*cos(x)+.1;
  /* return (exp (pow (x, 2))*(cos (x) + 2*x*sin (x)))/300.; */
}

double mm_ddu
( double x )
{
  return -mm_A*sin(x);
  /* return (exp (pow (x, 2))*(sin (x) + 4*x*(cos (x) + x*sin (x))))/300.; */
}

double mm_M
( double x )
{
  double u=mm_u(x);
  double du=mm_du(x);
  double ddu=mm_ddu(x);
  return fabs(du) + sqrt(fabs(ddu));
}

void mm_ODE_set
( void *	solver,
  double	t,
  double *	y,
  double *	f )
{
  ODE_solver *	s		= (ODE_solver*)solver;
  int		i,N		= s->params->Nx;
  int		initializedTest = 1;
  double	de,F,ddx,dx;
  double	crit		= 1.e-10;

  /* definicje zmiennych pomocniczych */
  de = 1./(N-1);

  for ( i = 1; i < N-1; i++) {
    F		     = (mm_M(y[i+1])-mm_M(y[i-1]))/2./de;
    dx		     = (y[i+1]-y[i-1])/2./de;
    ddx		     = (y[i+1]+y[i-1]-2*y[i])/de/de;
    f[i]	     = mm_M(y[i])*ddx + F*dx;
    initializedTest &= (fabs(f[i]) < crit);
  }

  /* boundary values */
  f[0] = 0.;
  f[N-1] = 0.;

  /* we are locally below the criticall value (|x_t| < crit everywhere) */
  if( initializedTest )
    {
      s->state->status = SOLVER_STATUS_STOP;
      return;
    }
}

/* This function uses the diffusion equation to relax the mesh to a
   density defined by the function mm_M. Literally it solves
   x_t = ddx + d(log(M))*dx untill rhs becomes smaller than some given value*/
void mm_setup_mesh ( double * x, int N )
{
  ODE_solver *		s;
  double		T	= 1.e10;
  double		x0	= 0., x1 = 1.;
  double		t_error = 1.e-14;
  h_basis_functions *	basis	= h_basis_finite_difference_5_function_init();
  gsl_odeiv_step_type * stepper = gsl_odeiv_step_rkf45;
  int i;
  FILE * F = fopen("test.dat","w");

  printf(RED1 "Relaxing mesh...\n" FORMAT_OFF);

  s = ODE_solver_init ( N, /*rk=*/ 1, T, x0, x1, t_error, basis, mm_ODE_set, NULL, stepper );
  /* ODE_modules_add ( s, ODE_module_print_time_init ( .0 ) ); */
  /* ODE_modules_add ( s, ODE_module_plot_init ( .0 ) ); */

  /* s->state->f=x; */

  for ( i = 0; i < N; i++ ) {
    s->state->f[i]=x[i];
  }

  ODE_solve(s);

  for ( i = 0; i < N; i++ ) {
    x[i]=s->state->f[i];
  }

  for ( i = 0; i < N; i++ ) {
    fprintf(F,"%2i %10.5G %10.5G\n",i,x[i],mm_M(x[i]));
  }

  fclose(F);

  ODE_solver_free(s);
  printf(RED1 "Mesh relaxed!\n" FORMAT_OFF);
}
