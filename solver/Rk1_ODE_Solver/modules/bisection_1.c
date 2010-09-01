#include "bisection_1.h"

void bisection_1_init ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  ODE_solver * s = (ODE_solver*)solver;
  bisection_1_module_data * data = (bisection_1_module_data*)m->data;

  data->result = 0.;
}


void bisection_1_step ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  ODE_solver * s = (ODE_solver*)solver;
  bisection_1_module_data * data = (bisection_1_module_data*)m->data;
  int N = (s->params->Nx-1)/2;
  H_DOUBLE * f = s->state->f;
  H_DOUBLE * u = f+1;
  H_DOUBLE * x = f+1+N;
  double x1 = s->params->basis->params->x1;


  /* if any of the bisection conditions are met, save the bisection
     result and stop the evolution */
  if( fabs(u[1]/x[1]) > 10. )
    {
      data->result = 1.;
      s->state->status = SOLVER_STATUS_STOP;
    }
  else if ( fabs(u[1]/x[1]) < 1. /* && u[N-2]/(x1-x[N-2]) > PI/2. */ )
    {
      data->result = -1.;
      s->state->status = SOLVER_STATUS_STOP;
    }
}

void bisection_1_stop ( void * solver, void * module )
{
}

void bisection_1_data_free ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  free( m->data );
}

ODE_module * ODE_module_bisection_1_init( H_DOUBLE dt )
{
  ODE_module * m = malloc ( sizeof ( ODE_module ) );
  bisection_1_module_data * data =
    malloc( sizeof(bisection_1_module_data) );

  m->data = data;

  m->init =
    &bisection_1_init;

  m->step =
    &bisection_1_step;

  m->stop =
    &bisection_1_stop;

  m->data_free =
    &bisection_1_data_free;

  /* negative value means not to execute step function */
  m->dt = dt;
  m->t_last = 0.;
  sprintf( m->name, "bisection");

  return m;
}
