#include "plot_sin.h"

void plot_sin_init ( void * solver, void * module )
{
/*   ODE_module * m = (ODE_module*)module; */
/*   plot_sin_module_data * data = (plot_sin_module_data*)m->data; */

/*   data->plot_sinter = gnuplot_sin_init(); */
/*   gnuplot_setstyle( data->plot_sinter, "lines" ); */
}

void plot_sin_step ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  ODE_solver * s = (ODE_solver*)solver;
  plot_sin_module_data * data = (plot_sin_module_data*)m->data;
  int N = s->params->Nx;

  gnuplot_ctrl * plotter = data->plotter;
  char * title = data->title;
  H_DOUBLE * t = s->state->t;
  int i;
  double x;

  N=(N-1)/2;

  for(i = 0; i < N-1; i++)
    {
      x=s->state->f[2+N+i];
      s->params->Dtemp[0][0][i]=s->state->f[2+i]/x;
      s->params->Dtemp[0][1][i]=log(x)/log(10.);
    }

  gnuplot_resetplot( plotter );
  /* set plot title */
  sprintf( title, "t = %.10E", s->state->f[0] );
  /* plot function */
  /* gnuplot_plot_xy( plotter, */
  /* 		   s->params->basis->collocation_points, */
  /* 		   s->state->f, */
  /* 		   s->params->Nx, */
  /* 		   title ); */
  gnuplot_plot_xy( plotter,
  		   s->params->Dtemp[0][1],
  		   s->params->Dtemp[0][0],
  		   N-1,
  		   title );
  /* gnuplot_plot_x( plotter, */
  /* 		  s->state->f+1+N, */
  /* 		  N, */
  /* 		  title ); */

}

void plot_sin_stop ( void * solver, void * module )
{
  /* ODE_module * m = (ODE_module*)module; */
  /* plot_sin_module_data * data = (plot_sin_module_data*)m->data; */

/*   gnuplot_close( data->plotter ); */
}

void plot_sin_data_free ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  plot_sin_module_data * data = (plot_sin_module_data*)m->data;

  gnuplot_close( data->plotter );
  free( m->data );
}

ODE_module * ODE_module_plot_sin_init ( H_DOUBLE dt )
{
  ODE_module * plot_sin_module = malloc ( sizeof ( ODE_module ) );
  plot_sin_module_data * data = malloc( sizeof(plot_sin_module_data) );

  data->plotter = gnuplot_init();
  gnuplot_setstyle( data->plotter, "linespoints" );
  /* gnuplot_cmd( data->plotter, "set logscale x\n" ); */

  plot_sin_module->data = data;

  plot_sin_module->init =
    &plot_sin_init;

  plot_sin_module->step =
    &plot_sin_step;

  plot_sin_module->stop =
    &plot_sin_stop;

  plot_sin_module->data_free =
    &plot_sin_data_free;

  plot_sin_module->dt = dt;
  plot_sin_module->t_last = 0.;

  return plot_sin_module;
}
