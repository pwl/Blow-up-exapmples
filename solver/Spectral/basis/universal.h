#ifndef _UNIVERSAL_H_
#define _UNIVERSAL_H_

#include "fftw3.h"
#include "Spectral/structs.h"


H_DOUBLE h_standard_interpolation ( void * basis, H_DOUBLE * f_i, H_DOUBLE x);

H_DOUBLE h_y_of_x( void * basis, H_DOUBLE x );

H_DOUBLE h_x_of_y( void * basis, H_DOUBLE y );

H_DOUBLE h_dy_over_dx( void * basis );

H_DOUBLE h_dx_over_dy( void * basis );

int h_standard_decompose( void * basis, H_DOUBLE * fi, H_DOUBLE * ai );

int h_standard_compose( void * basis, H_DOUBLE * fi, H_DOUBLE * ai );



#endif /*_UNIVERSAL_H_*/
