#ifndef _SHOOTING_H_
#define _SHOOTING_H_

#include "stdio.h"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
#include <math.h>

#define STEPPER gsl_odeiv_step_rkf45
#define STEPPER_ERROR 1.e-15
#define T_MAX 10.
#define PRINT_DT 1.e-4
#define PRINT_DT_RATIO 1.3
#define T0 1.e-14
#define H0 1.e-10

#define RIPPER_LINEAR 0x00
#define RIPPER_DENSE1 0x01
#define RIPPER_DENSE2 0x02

/* forward declaration of dimension */
double k;

int
ripper(double range_min,
       double range_max,
       double delta,
       unsigned int opt,
       int results_max,
       double * results,
       double val,
       double (*f)(double, int, char*, void *),
       void *);

double
bisec(double A0,
      double A1,
      double val,
      double e,
      double (*f)(double, int, char*, void *),
      void * p);

int
jac_dummy (double t, const double y[], double *dfdy,
	      double dfdt[], void *params);

/* Method used to calculate rhs of the equation for shrinking
   self-similar ansatz */
int
func_shrinker (double t, const double y[], double f[],
	       void *params);

/* Method used to calculate solution to shrinking self similar ansatz.
   Parameter p is derivative at 0. */
double
fevol_shrinker (double p, int print, char * filename, void * param);

/* Method used to calculate rhs of the linearized equation for
   eigenproblem of shrinking self-similar ansatz, lambda is passed as
   parameter. in this convention lambda is bounded from below */
int
func_shrinker_eigenproblem (double t, const double y[], double f[],
			    void *params);

/* Method used to calculate solution to the shrinking self-similar
   asnsatz, parameter lambda is proposed eigenvalue, in parameter p
   one passes the derivative of solution to NODE at 0 */
double
fevol_shrinker_eigenproblem (double lambda, int print, char * filename, void * p);



#endif /* _SHOOTING_H_ */
