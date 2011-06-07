#include "bisection_3.h"

void bisection_3_init ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  ODE_solver * s = (ODE_solver*)solver;
  bisection_3_module_data * data = (bisection_3_module_data*)m->data;

  data->gold = 0.;
  data->told = 0.;
  data->result = 0.;
}


void bisection_3_step ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  ODE_solver * s = (ODE_solver*)solver;
  bisection_3_module_data * data = (bisection_3_module_data*)m->data;
  int N = (s->params->Nx-1)/2;
  H_DOUBLE * f = s->state->f;
  H_DOUBLE * u = f+1;
  H_DOUBLE * x = f+1+N;
  H_DOUBLE t = s->state->f[0], told = data->told;
  H_DOUBLE g = (s->state->df[0]);
  H_DOUBLE dx = D1_at_0(u,x);/* D1(u,x,0,N); */
  double x1 = s->params->basis->params->x1;
  H_DOUBLE gold = data->gold;
  H_DOUBLE dgdt = fabs((g - gold)/(t-told));

  data->gold = g;
  data->told = t;

  /* printf("-log(g)=%.10G\n", -log(g)); */
  /* printf("1/dgdt=%.10G\n",1./dgdt/100.); */


  /* if any of the bisection conditions are met, save the bisection
     result and stop the evolution */
  if ( bisection_max(u,N) < PI/2. /* && u[N-2]/(x1-x[N-2]) > PI/2. */ )
    {
      data->result = -1.;
      s->state->status = SOLVER_STATUS_STOP;
    }
  /* if( 1./((fabs(u[1]/x[1])+1.)*pow(s->state->df[0]*100.,2.*0.630601937481871)) > 1000. && *s->state->t > 30. ) */
  else if (-log(g) > 15. && 1./dgdt < 100. )
    {
      data->result = 1.;
      s->state->status = SOLVER_STATUS_STOP;
    }
  /* else if ( u[1]/x[1] < mm_du(0.)/2. /\* && u[N-2]/(x1-x[N-2]) > PI/2. *\/ ) */

}

void bisection_3_stop ( void * solver, void * module )
{
}

void bisection_3_data_free ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  free( m->data );
}

ODE_module * ODE_module_bisection_3_init( H_DOUBLE dt )
{
  ODE_module * m = malloc ( sizeof ( ODE_module ) );
  bisection_3_module_data * data =
    malloc( sizeof(bisection_3_module_data) );

  m->data = data;

  m->init =
    &bisection_3_init;

  m->step =
    &bisection_3_step;

  m->stop =
    &bisection_3_stop;

  m->data_free =
    &bisection_3_data_free;

  /* negative value means not to execute step function */
  m->dt = dt;
  m->t_last = 0.;
  sprintf( m->name, "bisection");

  return m;
}

double bisection_max( double * v, int n)
{
  double max = 0.;
  int i = 0;

  for( i = 0; i < n; i++ )
    if(v[i] > max)
      max = v[i];
  return max;
}

