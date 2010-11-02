#include "gsl_evolver.h"

void gsl_evolver_init ( void * solver, void * module )
{
  /* nothing to initialize, mallocs are already done in the
     ODE_module_gsl_evolver_init */
}

void gsl_evolver_step ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  gsl_evolver_module_data * data =
    (gsl_evolver_module_data*)m->data;
  ODE_solver * s = (ODE_solver*)solver;

  gsl_odeiv_evolve_apply( data->e,
  			  data->c,
  			  data->s,
  			  data->sys,
  			  s->state->t,
  			  s->params->T,
  			  s->state->dt,
  			  s->state->f );
  /* gsl_odeiv_step_apply(   data->s, */
  /* 			  s->state->t[0], */
  /* 			  s->state->dt[0], */
  /* 			  s->state->f, */
  /* 			  s->params->Dtemp[0][0], */
  /* 			  NULL, */
  /* 			  s->state->df, */
  /* 			  data->sys ); */
  /* s->state->t[0]+=s->state->dt[0]; */
}

void gsl_evolver_stop ( void * solver, void * module )
{
  /* no need to do anything here */
  return;
}

/* TODO: memory allocated here leaks somehow */
ODE_module * ODE_module_gsl_evolver_init ( ODE_solver * s )
{
  ODE_module * m = malloc( sizeof( ODE_module ) );
  gsl_evolver_module_data * data = malloc( sizeof( gsl_evolver_module_data ) );

  data->sys = malloc( sizeof( gsl_odeiv_system ) );

  data->s =
    gsl_odeiv_step_alloc (s->params->stepper, s->params->Nx * s->params->rank);

  data->c =
    gsl_odeiv_control_y_new (s->params->t_error , s->params->t_error);

  data->e =
    gsl_odeiv_evolve_alloc (s->params->Nx * s->params->rank);

  /* state of the df points to the evolver array holding the updated
     values */
  s->state->df = data->e->dydt_out;

  data->sys->function = gsl_evolver_ODE_flow;
  data->sys->jacobian = gsl_evolver_jac;
  data->sys->dimension = s->params->Nx * s->params->rank;
  data->sys->params = (void*)s;

  m->data = (void*)data;

  m->init =
    &gsl_evolver_init;

  m->step =
    &gsl_evolver_step;

  m->stop =
    &gsl_evolver_stop;

  m->data_free =
    &gsl_evolver_data_free;

  /* we need to execute this always, so dt = 0. */
  m->dt = 0.;

  return m;
}

void gsl_evolver_data_free ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  gsl_evolver_module_data * data =
    (gsl_evolver_module_data*)(m->data);

  gsl_odeiv_evolve_free (data->e);
  gsl_odeiv_control_free (data->c);
  gsl_odeiv_step_free (data->s);
  free ( data->sys );
}

/* here are some helper functions, required by gsl */
int gsl_evolver_ODE_flow ( H_DOUBLE t,	       /* time */
			   const H_DOUBLE * y, /* value at time t */
			   H_DOUBLE * f, /* value of F(y) at time t */
			   void * solver ) /* pointer to ODE_solver */
{
  ODE_solver * s = (ODE_solver*)solver;

  /* this just redirects to the ODE_set defined in ODE_solver. the
     cast is to take off the "const" from y */
  s->params->ODE_set( s, t, (H_DOUBLE*)y, f );

  return GSL_SUCCESS;
}


/* phony jacobian */
int gsl_evolver_jac (double t, const double y[], double *dfdy, double dfdt[], void *solver)
{
  ODE_solver * s = (ODE_solver*)solver;

  if(!s->params->ODE_jac)
    return GSL_FAILURE;
  else
    {
      s->params->ODE_jac( s, t, y, dfdy, dfdt);
      return GSL_SUCCESS;
    }

  /* if called will throw out this message */
  /* printf("# This algorithm needs jacobian, please define it\n"); */
}
