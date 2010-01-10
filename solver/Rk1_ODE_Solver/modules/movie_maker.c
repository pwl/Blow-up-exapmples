#include "movie_maker.h"

void movie_maker_init ( void * solver, void * module )
{
}

void movie_maker_step ( void * solver, void * module )
{
  /* ODE_module * m = (ODE_module*)module; */
  ODE_solver * s = (ODE_solver*)solver;

  gnuplot_ctrl * plotter;
  char title[100];
  char output[100];
  H_DOUBLE * t = s->state->t;
  int N = (s->params->Nx-1)/2;
  int i;
  double x;

  plotter = gnuplot_init();

  sprintf( title, "set title \"bubbling off for harmonic maps\\nt=%.10E", s->state->f[0] );
  sprintf( output, "set output \"log/movie/movie_%.15f.png\"", *t );

  for(i = 0; i < N-2; i++)
    {
      x=s->state->f[2+N+i];
      s->params->Dtemp[0][0][i]=s->state->f[2+i]/sin(x);
      s->params->Dtemp[0][1][i]=log(tan(x/2.));
    }

  gnuplot_setstyle( plotter, "linespoints" );
  gnuplot_cmd( plotter, "set terminal png" );
  gnuplot_cmd( plotter, "set yrange [0:3*pi]" );
  gnuplot_cmd( plotter, title );
  gnuplot_cmd( plotter, "set xlabel \"log(tan(x/2))\"" );
  gnuplot_cmd( plotter, "set ylabel \"u(x)\"" );
  gnuplot_cmd( plotter, "set key off" );
  gnuplot_cmd( plotter, "set ytics (\"0\" 0, \"pi\" pi, \"2pi\" 2*pi, \"3pi\" 3*pi)" );
  gnuplot_cmd( plotter, "set grid" );



  gnuplot_cmd( plotter, output );

  /* gnuplot_cmd( plotter, "set trange [-1:1]" ); */

  gnuplot_resetplot( plotter );
  /* gnuplot_plot_xy( plotter, */
  /* 		   s->params->basis->collocation_points, */
  /* 		   s->state->f, */
  /* 		   s->params->Nx, */
  /* 		   title ); */
  gnuplot_plot_xy( plotter,
		   s->params->Dtemp[0][1],
  		   s->params->Dtemp[0][0],
  		   N-2,
  		   "" );
  /* gnuplot_cmd( plotter, "set parametric\n" ); */
  /* sprintf(title, "replot -(t*%f+t**3),t t \"self-sim\"\n", (1.-*t)); */
  /* gnuplot_cmd( plotter, title ); */
  /* gnuplot_cmd( plotter, "unset parametric\n" ); */


  gnuplot_close( plotter );
}

void movie_maker_stop ( void * solver, void * module )
{
}

ODE_module * ODE_module_movie_maker_init ( H_DOUBLE dt )
{
  ODE_module * movie_maker_module = malloc ( sizeof ( ODE_module ) );

  movie_maker_module->init =
    &movie_maker_init;

  movie_maker_module->step =
    &movie_maker_step;

  movie_maker_module->stop =
    &movie_maker_stop;

  movie_maker_module->data_free =
    NULL;

  movie_maker_module->dt = dt;
  movie_maker_module->t_last = .0;

  return movie_maker_module;
}
