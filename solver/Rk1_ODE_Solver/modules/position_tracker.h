#ifndef _POSITION_TRACKER_H_
#define _POSITION_TRACKER_H_

#include "Rk1_ODE_Solver/ODE_structs.h"

typedef struct 
{
  char out_file[100];
  H_DOUBLE a, da, pos_last, time_last;
} position_tracker_module_data;

/* standard module functions */
void position_tracker_init ( void * solver, void * module );

void position_tracker_step ( void * solver, void * module );

void position_tracker_stop ( void * solver, void * module );

/* module init function, can be programmed to take any arguments,
   and is called by the user */
ODE_module * ODE_module_position_tracker_init ( H_DOUBLE dt, H_DOUBLE a, H_DOUBLE da, char * out_file );


#endif /* _POSITION_TRACKER_H_ */
