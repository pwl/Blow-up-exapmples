#ifndef _FINITE_DIFFERENCE_5_H_
#define _FINITE_DIFFERENCE_5_H_

#include "Spectral/basis/universal.h"
/* #include "fftw3.h" */


H_DOUBLE h_finite_difference_5_cardinal( void * basis, int i, H_DOUBLE x );

int h_finite_difference_5_col_pts( void * basis, H_DOUBLE * xi );

int h_finite_difference_5_weights_gen( void * basis, H_DOUBLE * wi );

H_DOUBLE h_finite_difference_5_basis_nth( void * basis, int n, H_DOUBLE x );

h_basis_functions * h_basis_finite_difference_5_function_init ( void );

h_basis_params * h_finite_difference_5_params_init ( int N,
						     H_DOUBLE x0,
						     H_DOUBLE x1,
						     void * bassi_functions );

int h_basis_finite_difference_5_derivatives ( void * basis,
					      H_DOUBLE * fi,
					      H_DOUBLE * dfi,
					      H_DOUBLE * d2fi );


#endif /*_FINITE_DIFFERENCE_5_H_*/
