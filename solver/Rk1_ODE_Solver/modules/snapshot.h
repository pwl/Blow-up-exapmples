#ifndef _SNAPSHOT_H_
#define _SNAPSHOT_H_

#include "Rk1_ODE_Solver/ODE_structs.h"

/* includes specific to this module */

/* standard module functions */
void snapshot_init ( void * solver, void * module );

void snapshot_step ( void * solver, void * module );

void snapshot_stop ( void * solver, void * module );

/* module init function, can be programmed to take any arguments,
   and is called by the user */
ODE_module * ODE_module_snapshot_init ( H_DOUBLE dt );

double D1 ( double * u, double * x, int i, int N );
double D2 ( double * u, double * x, int i, int N );

double k;

#endif /* _SNAPSHOT_H_ */
