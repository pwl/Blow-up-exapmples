#ifndef _HARMONIC_H_
#define _HARMONIC_H_

#include "stdlib.h"

#include "solver/Rk1_ODE_Solver/ODE_solver.h"
#include "gsl/gsl_matrix.h"
#include "gsl/gsl_vector.h"
#include "gsl/gsl_permutation.h"
#include "gsl/gsl_blas.h"
#include "gsl/gsl_linalg.h"


void ODE_set ( void * solver,
	       H_DOUBLE t,
	       H_DOUBLE * y,
	       H_DOUBLE * f );

double D1 ( double * u, double * x, int i, int N );

double D2 ( double * u, double * x, int i, int N );

void M_calc ( double * u, double * x, double * M, int N );

double g ( double * y, int N );


#endif