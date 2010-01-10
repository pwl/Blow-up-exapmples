#ifndef _PRINT_TIME_H_
#define _PRINT_TIME_H_

#include "Rk1_ODE_Solver/ODE_structs.h"

/* standard module functions */
void print_time_init ( void * solver, void * module );

void print_time_step ( void * solver, void * module );

void print_time_stop ( void * solver, void * module );

/* module init function, can be programmed to take any arguments,
   and is called by the user */
ODE_module * ODE_module_print_time_init ( H_DOUBLE dt );


#endif /* _PRINT_TIME_H_ */
