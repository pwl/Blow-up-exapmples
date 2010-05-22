#include "bisection_2.h"

void bisection_2_init ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  ODE_solver * s = (ODE_solver*)solver;
  bisection_2_module_data * data = (bisection_2_module_data*)m->data;
  int N = s->params->Nx;
  
  data->f_max = PI/2.;
  data->f_min = 1.4;
  
  /* points where f_max/f_min are measured */
  data->i_max = N/16;
  data->i_min = N/2;
  
  data->result = 0.;  
}


void bisection_2_step ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  ODE_solver * s = (ODE_solver*)solver;
  bisection_2_module_data * data = (bisection_2_module_data*)m->data;
  H_DOUBLE * f, f_max, f_min;
  int i_max, i_min;
  
  f = s->state->f;
  i_max = data->i_max;
  i_min = data->i_min;
  f_max = data->f_max;
  f_min = data->f_min;
  
  /* if any of the bisection conditions are met, save the bisection
     result and stop the evolution */
  if( f[i_max] > f_max )
    {
      data->result = 1.;
      s->state->status = SOLVER_STATUS_STOP;
    }  
  else if ( f[i_min] < f_min )
    {
      data->result = -1.;
      s->state->status = SOLVER_STATUS_STOP;
    }
}

void bisection_2_stop ( void * solver, void * module )
{
}
 
void bisection_2_data_free ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  free( m->data );
}

ODE_module * ODE_module_bisection_2_init( H_DOUBLE dt )
{
  ODE_module * m = malloc ( sizeof ( ODE_module ) );
  bisection_2_module_data * data = 
    malloc( sizeof(bisection_2_module_data) );
  
  m->data = data;
  
  m->init = 
    &bisection_2_init;
  
  m->step =
    &bisection_2_step;
  
  m->stop = 
    &bisection_2_stop;
  
  m->data_free = 
    &bisection_2_data_free;
  
  /* negative value means not to execute step function */
  m->dt = dt;
  m->t_last = 0.;
  sprintf( m->name, "bisection");
  
  return m;
}
