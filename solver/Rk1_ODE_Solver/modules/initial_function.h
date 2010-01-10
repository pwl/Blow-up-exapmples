#ifndef _INITIAL_FUNCTION_H_
#define _INITIAL_FUNCTION_H_

#include "Rk1_ODE_Solver/ODE_structs.h"


/* data specific to this module */
typedef struct
{
  H_DOUBLE (**f)(H_DOUBLE, H_DOUBLE);
  H_DOUBLE A;
} initial_function_module_data;

/* standard module functions */
void initial_function_init ( void * solver, void * module );

void initial_function_step ( void * solver, void * module );

void initial_function_stop ( void * solver, void * module );

void initial_function_data_free ( void * solver, void * module );

/* module init function, can be programmed to take any arguments,
   and is called by the user */
ODE_module * ODE_module_initial_function_init (H_DOUBLE (**f)(H_DOUBLE, H_DOUBLE),
					       H_DOUBLE A );



#endif /* _INITIAL_FUNCTION_H_ */
