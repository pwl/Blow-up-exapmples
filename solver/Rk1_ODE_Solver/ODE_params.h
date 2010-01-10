#ifndef _ODE_PARAMS_H_
#define _ODE_PARAMS_H_

#include "ODE_structs.h"

#define DTEMP_DMAX 10		/**< This parameter sets the maximum
				   number of derivatives to allocate
				   memory for */

void ODE_params_init ( ODE_solver * s,
		       int N,
		       int rk,
		       H_DOUBLE T,
		       H_DOUBLE x0,
		       H_DOUBLE x1,
		       /* 			       H_DOUBLE * f0, */
		       H_DOUBLE t_error,
		       h_basis_functions * basis_type,
		       void (*ODE_set)(void * ODE_solver, H_DOUBLE t, H_DOUBLE * y, H_DOUBLE * f ),
		       gsl_odeiv_step_type * stepper/*  = gsl_odeiv_step_rk2  */);

void ODE_params_free ( ODE_solver * s );



#endif /*_ODE_PARAMS_H_*/
