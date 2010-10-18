#include "Rk1_ODE_Solver/ODE_solver.h"

ODE_solver * ODE_solver_malloc ( void )
{
  return malloc( sizeof( ODE_solver ) );
}


ODE_solver * ODE_solver_init ( int N,
			       int rk,
			       H_DOUBLE T,
			       H_DOUBLE x0, H_DOUBLE x1,
			       H_DOUBLE t_error,
			       h_basis_functions * basis_type,
			       void (*ODE_set)(void * ODE_solver, H_DOUBLE t, H_DOUBLE * y, H_DOUBLE * f ),
			       void (*ODE_jac)(void * ODE_solver, H_DOUBLE t, H_DOUBLE * y, H_DOUBLE * dfdy, H_DOUBLE * dfdt ),
			       gsl_odeiv_step_type * stepper )
{
  ODE_solver * s = ODE_solver_malloc ();

  /* intialize parameters struct first, becouse most of the
     initialization functions will assume the s->params is already
     defined */

  ODE_params_init ( s, N, rk, T, x0, x1, t_error, basis_type, ODE_set, ODE_jac, stepper );
  ODE_state_init ( s );
  ODE_modules_list_init ( s );

/*   it is necessary to add at least one evolver-type module */
  ODE_modules_add( s, ODE_module_gsl_evolver_init ( s ) );

  return s;
}


void ODE_solver_free ( ODE_solver * s )
{
  ODE_modules_list_free ( s );
  ODE_params_free ( s );
  ODE_state_free ( s );
  free ( s );
}


/* ODE SOLVING PART */

void ODE_solve ( ODE_solver * s )
{
  ODE_modules_execute_init ( s );

  while ( s->state->status == SOLVER_STATUS_OK &&
	  ( *(s->state->t) < s->params->T ) )
    ODE_modules_execute_step ( s );

  ODE_modules_execute_stop ( s );
}


/**
 * This function fills in the first derivative slots of
 * s->params->Dtemp using the fi as function values.
 *
 * @param s
 * @param fi function values
 */
void ODE_derivatives_D1( ODE_solver * s, H_DOUBLE * fi )
{
  int rk = s->params->rank;
  int Nx = s->params->Nx;
  int i;
  H_DOUBLE *** Dtemp = s->params->Dtemp;
  h_basis * b = s->params->basis;

  for ( i = 0; i < rk; i++ )
    {
      b->functions->df_dx( b, fi + Nx*i, Dtemp[i][0] );
    }
}

/**
 * This functions fills in the first and second derivative slots of
 * s->params->Dtemp using the fi as function values.
 *
 * @param s
 * @param fi function values
 */
void ODE_derivatives_D2( ODE_solver * s, H_DOUBLE * fi )
{
  int rk = s->params->rank;
  int Nx = s->params->Nx;
  int i;
  H_DOUBLE *** Dtemp = s->params->Dtemp;
  h_basis * b = s->params->basis;


  for ( i = 0; i < rk; i++ )
    {
      b->functions->derivatives( b, fi + Nx*i, Dtemp[i][0], Dtemp[i][1] );
    }
}
