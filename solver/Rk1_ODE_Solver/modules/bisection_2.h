#ifndef _BISECTION_2_H_
#define _BISECTION_2_H_

#include "../ODE_structs.h"

/* data specific to this module */
typedef struct
{
  H_DOUBLE f_max, f_min, result;
  int i_max, i_min;
} bisection_2_module_data;

/* standard module functions */
void bisection_2_init ( void * solver, void * module );

void bisection_2_step ( void * solver, void * module );

void bisection_2_stop ( void * solver, void * module );

void bisection_2_data_free ( void * solver, void * module );

/* module init function, can be programmed to take any arguments,
   and is called by the user */
ODE_module * ODE_module_bisection_2_init (H_DOUBLE dt);

#endif /* _BISECTION_2_H_ */
