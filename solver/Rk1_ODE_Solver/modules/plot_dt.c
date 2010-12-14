#include "plot_dt.h"
#include "assert.h"


void plot_dt_init ( void * solver, void * module )
{
/*   ODE_module * m = (ODE_module*)module; */
/*   plot_dt_module_data * data = (plot_dt_module_data*)m->data; */

/*   data->plotter = gnuplot_dt_init(); */
/*   gnuplot_dt_setstyle( data->plotter, "lines" ); */
}

void plot_dt_step ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  ODE_solver * s = (ODE_solver*)solver;
  plot_dt_module_data * data = (plot_dt_module_data*)m->data;
  int N = s->params->Nx;

  gnuplot_ctrl * plotter = data->plotter;
  char * title = data->title;
  H_DOUBLE * t = s->state->t;
  int i;
  double x;
  double x1 = s->params->basis->params->x1;


  N=(N-1)/2;

  for(i = 0; i < N*2+1; i++)
    {
      s->params->Dtemp[0][0][i]=fabs(s->state->df[i])/* /x */;
    }


  gnuplot_resetplot( plotter );
  /* set plot title */
  sprintf( title, "t = %1.15G", s->state->f[0] );
  /* plot function */
  /* gnuplot_plot_xy( plotter, */
  /* 		   s->params->basis->collocation_points, */
  /* 		   s->state->f, */
  /* 		   s->params->Nx, */
  /* 		   title ); */
  /* gnuplot_plot_xy( plotter, */
  /* 		   s->params->Dtemp[0][1]+1, */
  /* 		   s->params->Dtemp[0][0]+1, */
  /* 		   N-2, */
  /* 		   title ); */
  gnuplot_plot_x( plotter,
  		  s->params->Dtemp[0][0]/* +1+N */,
  		  N*2+1,
  		  title );

}

void plot_dt_stop ( void * solver, void * module )
{
  /* ODE_module * m = (ODE_module*)module; */
  /* plot_dt_module_data * data = (plot_dt_module_data*)m->data; */

/*   gnuplot_close( data->plotter ); */
}

void plot_dt_data_free ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  plot_dt_module_data * data = (plot_dt_module_data*)m->data;

  gnuplot_close( data->plotter );
  free( m->data );
}

ODE_module * ODE_module_plot_dt_init ( H_DOUBLE dt )
{
  ODE_module * plot_dt_module = malloc ( sizeof ( ODE_module ) );
  plot_dt_module_data * data = malloc( sizeof(plot_dt_module_data) );

  data->plotter = gnuplot_init();
  gnuplot_setstyle( data->plotter, "linespoints" );
  gnuplot_cmd( data->plotter, "set yrange [1e-6:1e3]; set logscale y 10\n" );

  plot_dt_module->data = data;

  plot_dt_module->init =
    &plot_dt_init;

  plot_dt_module->step =
    &plot_dt_step;

  plot_dt_module->stop =
    &plot_dt_stop;

  plot_dt_module->data_free =
    &plot_dt_data_free;

  plot_dt_module->dt = dt;
  plot_dt_module->t_last = 0.;

  return plot_dt_module;
}
