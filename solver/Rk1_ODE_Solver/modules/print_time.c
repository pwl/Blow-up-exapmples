#include "print_time.h"


void print_time_init ( void * solver, void * module )
{
  printf("\n");
}

void print_time_step ( void * solver, void * module )
{
  ODE_solver * s = (ODE_solver*)solver;
  int i, bar_length=100;
  H_DOUBLE t = *s->state->t;
  H_DOUBLE T = s->params->T;

  /* TODO: add options to turn on/off features of the status-bar */

  /* progress bar */
  printf("[");
  for( i = 0; i<bar_length; i++ )
    printf( i < (int)(t/T*bar_length)?"#":" ");

  printf("] %3.2f%% ", t/T*100.);

  /* print time */
  printf("t = %.15E",t);

  printf("\r");

}

void print_time_stop ( void * solver, void * module )
{
  printf("\n");
}


ODE_module * ODE_module_print_time_init ( H_DOUBLE dt )
{
  ODE_module * print_time_module = malloc ( sizeof ( ODE_module ) );

  print_time_module->init =
    &print_time_init;

  print_time_module->step =
    &print_time_step;

  print_time_module->stop =
    &print_time_stop;

  print_time_module->data_free =
    NULL;

  print_time_module->t_last=0.;
  print_time_module->dt = dt;

  return print_time_module;
}
