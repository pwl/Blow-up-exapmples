#ifndef _HARMONIC_H_
#define _HARMONIC_H_

#include "stdlib.h"
#include "assert.h"

#define __USE_BSD

#include "math.h"
#include "solver/Rk1_ODE_Solver/ODE_solver.h"
#include "omp.h"
#include "gsl/gsl_matrix.h"
#include "gsl/gsl_vector.h"
#include "gsl/gsl_permutation.h"
#include "gsl/gsl_blas.h"
#include "gsl/gsl_linalg.h"

#include "mm_distribute_points.h"
#include "derivatives.h"

#define min(a,b) a<b?a:b
#define max(a,b) a>b?a:b

void ODE_set ( void * solver,
	       H_DOUBLE t,
	       H_DOUBLE * y,
	       H_DOUBLE * f );

void M_calc ( double * u, double * x, double * M, int N );

double g ( double * y, int N );

double bisection_wrapper(double A, void * p);

double
bisec(double A0,
      double A1,
      double e,
      double val,
      double (*fevol)(double, void *),
      void * param);

double mAnalyical (double x);

void M_smoothen ( double * M, double * Mtemp, int N, double gamma, int ip );


#endif
