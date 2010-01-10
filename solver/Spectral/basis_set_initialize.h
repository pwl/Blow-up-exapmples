#ifndef _BASIS_SET_INITIALIZE_H_
#define _BASIS_SET_INITIALIZE_H_

#include "structs.h"

h_basis * h_basis_initialize (int N, H_DOUBLE x0, H_DOUBLE x1,
			      h_basis_functions * type );

void h_basis_tables_alloc ( h_basis * b );

void h_basis_tables_free ( h_basis * b );

void h_basis_free ( h_basis * b );

void h_params_free ( h_basis_params * p );



#endif /*_BASIS_SET_INITIALIZE_H_*/
