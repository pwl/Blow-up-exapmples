#include "ODE_state.h"

ODE_state * ODE_state_malloc ( ODE_solver * s )
{
  int N = s->params->Nx, rk = s->params->rank;

  ODE_state * state = malloc( sizeof( ODE_state ) );

  state->f  = malloc( rk * N * sizeof( H_DOUBLE ) );
  /* temporary not allocating memory to df, becouse it points directly
     to the otherwise inaccessilbe internal gsl structure, from the
     same reason it isn't freed also */

  state->t  = malloc( sizeof( H_DOUBLE ) );
  state->dt = malloc( sizeof( H_DOUBLE ) );

  return state;
}

void ODE_state_init ( ODE_solver * s )
{
  int i, N = s->params->Nx, rk = s->params->rank;
  s->state = ODE_state_malloc ( s );

  /* setup some initial values */
  *s->state->t = 0.;
  *s->state->dt= 1.e-10;		/* TODO: change into something reasonable */
  s->state->status = SOLVER_STATUS_OK;

  /* this fill the function-values with zeros, should be removed
     anyway */
  for ( i = 0; i < N*rk; i++ )
    s->state->f[i] = 0.;	/* doesn't matter anyway, is changed
				   by the module now */
}


void ODE_state_free ( ODE_solver * s )
{
  ODE_state * state = s->state;

  free( state->f );
  /* state->df does not require to be freed becouse it points to an
     external (gsl) array */
  free( state->t );
  free( state->dt );
  free( state );
}
