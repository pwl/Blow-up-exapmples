#ifndef _PLOT_DT_H_
#define _PLOT_DT_H_

#include "Rk1_ODE_Solver/ODE_structs.h"

/* includes specific to this module */
#include "gnuplot_i.h"

/* data specific to this module */
typedef struct
{
  gnuplot_ctrl * plotter;
  char title[100];
} plot_dt_module_data;

/* standard module functions */
void plot_dt_init ( void * solver, void * module );

void plot_dt_step ( void * solver, void * module );

void plot_dt_stop ( void * solver, void * module );

void plot_dt_data_free ( void * solver, void * module );

/* module init function, can be programmed to take any arguments,
   and is called by the user */
ODE_module * ODE_module_plot_dt_init ( H_DOUBLE dt );

#endif /* _PLOT_DT_H_ */
