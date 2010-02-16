#ifndef _SHOOTING_H_
#define _SHOOTING_H_

#include "stdio.h"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
#include <math.h>

#define PI		3.14159265358979323846	/* pi */

#define STEPPER gsl_odeiv_step_rk8pd
#define STEPPER_ERROR 1.e-15
#define T_MAX 10.
#define PRINT_DT 1.e-4
#define PRINT_DT_RATIO 1.3
#define T0 1.e-10
#define H0 1.e-14
#define RIPPER_BISEC_EPSILON 1.e-15
#define HARVESTER_DATA_DIR "harvester_data/"
#define HARVESTER_DEFAULT_EXTENSION "_k%.5f_l%.1f.dat"
#define HARVESTER_DEFAULT_EIGEN_EXTENSION "_k%.5f_l%.1f_i%i.dat"

#define RIPPER_LINEAR 0x00
#define RIPPER_DENSE1 0x01
#define RIPPER_DENSE2 0x02

#define CYAN1 "\x1b[0;36m"
#define RED1 "\x1b[1;31m"
#define GREEN1 "\x1b[1;32m"
#define BOLD1 "\x1b[1m"
#define FORMAT_OFF "\x1b[0m"

/* forward declaration of dimension */
double k;
/* eigenvalue index of the laplacian on S^(n-1), lambda = l*(l+n-2) */
double l;

void print_shrinker_profile( double A );

int
harvester(double range_min,
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
   Parameter A is defined by the expansion of solution around 0.,
   namely f(x)=x^l*(A+...) */
double
fevol_shrinker (double A, int print, char * filename, void * param);

/* Method used to calculate rhs of the linearized equation for
   eigenproblem of shrinking self-similar ansatz, lambda is passed as
   parameter. in this convention lambda is bounded from below */
int
func_shrinker_eigenproblem (double t, const double y[], double f[],
			    void *params);

/* Method used to calculate solution to the shrinking self-similar
   asnsatz, parameter lambda is proposed eigenvalue, in parameter A
   one passes the parameter A from the solution is of the form
   x^l*(A+...) */
double
fevol_shrinker_eigenproblem (double lambda, int print, char * filename, void * A);

/* this solves the eigenvalue equation for shrinker given parameter A
   as above */
void
solve_shrinker_eigenproblem (double A, int index);

/* function below is used to print the profile of a shrinker to
   apropriate file */
void
print_shrinker_profile( double A );

int
func_expander (double t, const double y[], double f[],
	       void *params);
double
fevol_expander (double bisec_param, int print, char * filename, void * p);

/* functions used to generate static solutions to harmonic map flow */
int
func_static (double t, const double y[], double f[],
	     void *params);

/* L is the length of the interval the function is calculated on */
double
fevol_static (double L, int print, char * filename, void * p);



#endif /* _SHOOTING_H_ */
