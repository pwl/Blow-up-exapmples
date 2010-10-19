#include "ODE_params.h"


void ODE_params_init ( ODE_solver * s,
		       int N,
		       int rk,
		       H_DOUBLE T,
		       H_DOUBLE x0, H_DOUBLE x1,
		       H_DOUBLE t_error,
		       h_basis_functions * basis_type,
		       void (*ODE_set)(void * ODE_solver, H_DOUBLE t, H_DOUBLE * y, H_DOUBLE * f ),
		       void (*ODE_jac)(void * ODE_solver, H_DOUBLE t, H_DOUBLE * y, H_DOUBLE * dfdy, H_DOUBLE * dfdt ),
		       gsl_odeiv_step_type * stepper/*  = gsl_odeiv_step_rk8pd */ )
{
  int i,j;
  /* TODO: Move this definition out of the body of the function */
  int Dmax = DTEMP_DMAX;		/**< Dmax is the maximal number of
				   derivatives that Dtemp can hold */

  s->params = malloc( sizeof( ODE_static ) );
  s->params->rank = rk;
  s->params->T = T;
  s->params->Nx = N;
  s->params->t_error = t_error;
  s->params->basis = h_basis_initialize( N, x0, x1, basis_type );

  s->params->ODE_set = ODE_set;
  s->params->stepper = stepper;

  /* Allocate the memory for the temporary storage of derivatives, for
     explanation see ODE_params->Dtemp */
  s->params->Dtemp = malloc( sizeof( H_DOUBLE ** ) * rk );
  for ( i = 0; i < rk; i++ )
    {
      s->params->Dtemp[i] = malloc( sizeof( H_DOUBLE * ) * Dmax );
      for ( j = 0; j < Dmax; j++ )
  	s->params->Dtemp[i][j] = malloc( sizeof( H_DOUBLE ) * N );
    }

  /*  Method one of the following:
      gsl_odeiv_step_rk2
      gsl_odeiv_step_rk4
      gsl_odeiv_step_rkf45
      gsl_odeiv_step_rkck
      gsl_odeiv_step_rk8pd
      gsl_odeiv_step_rk2imp
      gsl_odeiv_step_rk4imp
      gsl_odeiv_step_bsimp
      gsl_odeiv_step_gear1
      gsl_odeiv_step_gear2
  */
}

void ODE_params_free ( ODE_solver * s )
{
  ODE_static * params = s->params;

  /* TODO: Move this definition out of the body of the function */
  int Dmax = DTEMP_DMAX; /**< Dmax is the maximal number of
				   derivatives that Dtemp can hold */
  int rk = s->params->rank;
  int i,j;

  for ( i = 0; i < rk; i++ )
    {
      for ( j = 0; j < Dmax; j++ )
  	free( params->Dtemp[i][j] );
      free( params->Dtemp[i] );
    }
  free( params->Dtemp );

  h_basis_free ( params->basis );

  free( params );
}
