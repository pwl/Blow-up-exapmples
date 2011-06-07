#ifndef _BISECTION_3_H_
#define _BISECTION_3_H_

#include "../ODE_structs.h"
#include "../../../mm_distribute_points.h"

/* data specific to this module */
typedef struct
{
  H_DOUBLE f_max, f_min, result;
  int i_max, i_min;
} bisection_3_module_data;

/* standard module functions */
void bisection_3_init ( void * solver, void * module );

void bisection_3_step ( void * solver, void * module );

void bisection_3_stop ( void * solver, void * module );

void bisection_3_data_free ( void * solver, void * module );

double bisection_max( double * v, int n);

/* module init function, can be programmed to take any arguments,
   and is called by the user */
ODE_module * ODE_module_bisection_3_init (H_DOUBLE dt);

#endif /* _BISECTION_3_H_ */
