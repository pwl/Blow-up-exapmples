#ifndef _SHOOTING2_H_
#define _SHOOTING2_H_

#include "shooting.h"
#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_deriv.h>
#include <math.h>


double lambda_at_given_A(double A, int z, char * f, void * ptr);
double lambda_0_at_given_A(double A, int z, char * f, void * ptr);
double asymptotic_gsl_wrapper( double A, void * ptr );
double d1_asymptotic_gsl_wrapper
( double A,
  int i,
  char * c,
  void * ptr );


#endif /* _SHOOTING2_H_ */
