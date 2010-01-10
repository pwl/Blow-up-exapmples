#ifndef _GSL_EVOLVER_H_
#define _GSL_EVOLVER_H_


#include "Rk1_ODE_Solver/ODE_structs.h"

/* gsl libraries to include */
#include "gsl/gsl_odeiv.h"
#include "gsl/gsl_errno.h"

typedef struct
{
  gsl_odeiv_step_type * stepper;
  gsl_odeiv_step * s;
  gsl_odeiv_control * c;
  gsl_odeiv_evolve * e;
  gsl_odeiv_system * sys;
} gsl_evolver_module_data;

void gsl_evolver_init ( void * solver, void * module );

void gsl_evolver_step ( void * solver, void * module );

void gsl_evolver_stop ( void * solver, void * module );

void gsl_evolver_data_free ( void * solver, void * module );

/* module init function, can be programmed to take any arguments,
   and is called by the user */
ODE_module * ODE_module_gsl_evolver_init ( ODE_solver * );


/* additional helper functions */
int gsl_evolver_ODE_flow ( H_DOUBLE t,
			   const H_DOUBLE * y,
			   H_DOUBLE * f,
			   void * solver );

int gsl_evolver_jac (double t, const double y[], double *dfdy, double dfdt[], void *params);


#endif /* _GSL_EVOLVER_H_ */
