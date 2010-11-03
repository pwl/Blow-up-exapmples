#include "plot_x.h"

void plot_x_init ( void * solver, void * module )
{
/*   ODE_module * m = (ODE_module*)module; */
/*   plot_x_module_data * data = (plot_x_module_data*)m->data; */

/*   data->plot_sinter = gnuplot_sin_init(); */
/*   gnuplot_setstyle( data->plot_sinter, "lines" ); */
}

void plot_x_step ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  ODE_solver * s = (ODE_solver*)solver;
  plot_x_module_data * data = (plot_x_module_data*)m->data;
  int N = s->params->Nx;

  gnuplot_ctrl * plotter = data->plotter;
  char * title = data->title;
  H_DOUBLE * t = s->state->t;
  int i;
  double x;
  double * f=s->state->f;
  double * df=s->state->df;

  N=(N-1)/2;

  for(i = 1; i < N; i++)
    {
      x=s->state->f[i+N+1];
      s->params->Dtemp[0][0][i]=s->state->f[1+i]/x;
      s->params->Dtemp[0][1][i]=x/sqrt(df[0]);
    }

  s->params->Dtemp[0][0][0]=D1(f+1,f+N+1,0,N);
  s->params->Dtemp[0][1][0]=f[1+N];

  gnuplot_resetplot( plotter );
  /* set plot title */
  sprintf( title, "t = %1.10E", s->state->f[0] );
  /* plot function */
  /* gnuplot_plot_xy( plotter, */
  /* 		   s->params->basis->collocation_points, */
  /* 		   s->state->f, */
  /* 		   s->params->Nx, */
  /* 		   title ); */
  gnuplot_plot_xy( plotter,
  		   s->params->Dtemp[0][1],
  		   s->params->Dtemp[0][0],
  		   N,
  		   title );
  /* gnuplot_plot_x( plotter, */
  /* 		  s->state->f+1+N, */
  /* 		  N, */
  /* 		  title ); */

}

void plot_x_stop ( void * solver, void * module )
{
  /* ODE_module * m = (ODE_module*)module; */
  /* plot_sin_module_data * data = (plot_sin_module_data*)m->data; */

/*   gnuplot_close( data->plotter ); */
}

void plot_x_data_free ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  plot_x_module_data * data = (plot_x_module_data*)m->data;

  gnuplot_close( data->plotter );
  free( m->data );
}

ODE_module * ODE_module_plot_x_init ( H_DOUBLE dt )
{
  ODE_module * plot_x_module = malloc ( sizeof ( ODE_module ) );
  plot_x_module_data * data = malloc( sizeof(plot_x_module_data) );

  data->plotter = gnuplot_init();
  gnuplot_setstyle( data->plotter, "linespoints" );
  /* gnuplot_cmd( data->plotter, "set xrange [:]\n" ); */
  gnuplot_cmd( data->plotter, "set yrange [0:3*pi]\n" );
  gnuplot_cmd( data->plotter, "set logscale x\n" );

  plot_x_module->data = data;

  plot_x_module->init =
    &plot_x_init;

  plot_x_module->step =
    &plot_x_step;

  plot_x_module->stop =
    &plot_x_stop;

  plot_x_module->data_free =
    &plot_x_data_free;

  plot_x_module->dt = dt;
  plot_x_module->t_last = 0.;

  return plot_x_module;
}
