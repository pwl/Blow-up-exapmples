#ifndef _ODE_SOLVER_H_
#define _ODE_SOLVER_H_

#include "Rk1_ODE_Solver/modules/gsl_evolver.h"
#include "Rk1_ODE_Solver/modules/plot_sin.h"
#include "Rk1_ODE_Solver/modules/plot.h"
#include "Rk1_ODE_Solver/modules/initial_function.h"
#include "Rk1_ODE_Solver/modules/print_time.h"
#include "Rk1_ODE_Solver/modules/info_1.h"
#include "Rk1_ODE_Solver/modules/bisection_3.h"
#include "Rk1_ODE_Solver/modules/position_tracker.h"
#include "Rk1_ODE_Solver/modules/snapshot.h"
#include "Rk1_ODE_Solver/modules/movie_maker.h"
#include "Rk1_ODE_Solver/ODE_params.h"
#include "Rk1_ODE_Solver/ODE_state.h"
#include "Rk1_ODE_Solver/ODE_modules.h"


ODE_solver * ODE_solver_malloc ( void );

ODE_solver * ODE_solver_init ( int N,
			       int rk,
			       H_DOUBLE T,
			       H_DOUBLE x0, H_DOUBLE x1,
			       H_DOUBLE t_error,
			       h_basis_functions * basis,
			       void (*ODE_set)(void * ODE_solver, H_DOUBLE t, H_DOUBLE * y, H_DOUBLE * f ),
			       gsl_odeiv_step_type * stepper );

void ODE_solver_free ( ODE_solver * s );

void ODE_solve ( ODE_solver * s );

void ODE_derivatives_D1( ODE_solver * s, H_DOUBLE * fi );

void ODE_derivatives_D2( ODE_solver * s, H_DOUBLE * fi );


#endif /*_ODE_SOLVER_H_*/
