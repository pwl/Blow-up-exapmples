#ifndef _ODE_RK1_STRUCTS_H_
#define _ODE_RK1_STRUCTS_H_

/* include available basis here */
#include "Spectral/spectral.h"

/* include modules */
#include "Rk1_ODE_Solver/modules/modules.h"

#include "gsl/gsl_odeiv.h"


/* ODE_params structure contains all the data that is not going to
   change during the evolution and change of which will require the
   reinitialization of ODE_solver */
typedef struct
{
  H_DOUBLE t_error;	 /**< maximal time marching step error */
  H_DOUBLE T;		 /**< time of simulation */
  int rank;		 /**< rank of the ODE to solve */
  int Nx;		 /**< number of space points used */

  const gsl_odeiv_step_type * stepper; /**< stepper function used in
					  time marching */
  h_basis * basis;	 /**< basis used to calculate derivatives
			    (same basis for every rank) */
  H_DOUBLE *** Dtemp;		/**< Dtemp points to the memory
				   allocated to hold derivatives
				   temporarly. E.g. Dtemp[0][1][2] is
				   a second derivative at third
				   collocation point of first of the
				   n, n-th rank ODE, independent
				   functions */
  /* the set of ODE equations used to evolve initial values */
  void (*ODE_set)(void * ODE_solver, H_DOUBLE t, H_DOUBLE * y, H_DOUBLE * f );
  void (*ODE_jac)(void * ODE_solver, H_DOUBLE t, H_DOUBLE * y, H_DOUBLE * dfdy, H_DOUBLE * dfdt );
} ODE_static;			/**< This structure holds the
				   variables that are not going to be
				   changed during runtime of
				   ODE_Solver */


/* pointers to values that change during the time marching stage,
   updated by the solver */
typedef struct
{
  H_DOUBLE * f;			/**< Values of the function at current
				   time. The table consists of n
				   parts, where n is the rank of the
				   ODE solve */
  H_DOUBLE * df;		/**< Values of the first derivative of
				   a function at current time, is
				   partitioned as f */
  H_DOUBLE * t;			/**< Current time */
  H_DOUBLE * dt;		/**< Current time step */
  enum status
    { SOLVER_STATUS_OK,
      SOLVER_STATUS_STOP } status; /**< status of the solver */
} ODE_state;			   /**< This structure contains the
				   current state of the ODE set. The
				   most important part of its
				   definition is the f table,
				   containing the values of the
				   evolved function at given time. */

/* structure used to hold the list of loaded modules */
typedef struct
{
  ODE_module ** mod_list;	/* list of modules to load */
  int n;			/* number of modules loaded */
} ODE_modules_list;

/* this structure holds all the main parts of the solver */
typedef struct
{
  ODE_static * params;
  ODE_modules_list * modules;
  ODE_state * state;
} ODE_solver;

#endif /* _ODE_RK1_STRUCTS_H_ */
