#include "initial_function.h"

void initial_function_init ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  ODE_solver * s = (ODE_solver*)solver;
  initial_function_module_data * data = (initial_function_module_data*)m->data;
  H_DOUBLE * x = s->params->basis->collocation_points;
  int i,j,N = s->params->Nx, rk = s->params->rank;

  for ( j = 0; j < rk; j++)
    for( i = 0; i < N; i++ )
      s->state->f[i+N*j]=data->f[j](x[i],data->A);
}


void initial_function_step ( void * solver, void * module )
{
}

void initial_function_stop ( void * solver, void * module )
{
}
 
void initial_function_data_free ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  free( m->data );
}

ODE_module * ODE_module_initial_function_init( H_DOUBLE (**f)(H_DOUBLE, H_DOUBLE),
					       H_DOUBLE A )
{
  ODE_module * m = malloc ( sizeof ( ODE_module ) );
  initial_function_module_data * data = 
    malloc( sizeof(initial_function_module_data) );
  
  data->f = f;
  data->A = A;
  
  m->data = data;
  
  m->init = 
    &initial_function_init;
  
  m->step =
    &initial_function_step;
  
  m->stop = 
    &initial_function_stop;
  
  m->data_free = 
    &initial_function_data_free;
  
  /* negative value means not to execute step function */
  m->dt = -1.;
  m->t_last = 0.;
  sprintf(m->name, "f0");
  
  return m;
}
