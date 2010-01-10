#ifndef _MOVIE_MAKER_H_
#define _MOVIE_MAKER_H_

#include "Rk1_ODE_Solver/ODE_structs.h"

/* includes specific to this module */
#include "gnuplot_i.h"

/* standard module functions */
void movie_maker_init ( void * solver, void * module );

void movie_maker_step ( void * solver, void * module );

void movie_maker_stop ( void * solver, void * module );

/* module init function, can be programmed to take any arguments,
   and is called by the user */
ODE_module * ODE_module_movie_maker_init ( H_DOUBLE dt );

#endif /* _MOVIE_MAKER_H_ */
