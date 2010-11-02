#include "plot.h"
#include "assert.h"

double D1 ( double * u, double * x, int i, int N );

/* obliczanie pierwszej pochodnej */
double _D1 ( double * u, double * x, int i, int N )
{
  double du;

  /* for ( i = 0; i < N; i++ ) */
  /*   printf("%i %.15f %.15f\n", i, x[i], u[i]); */

  if( i > 0 && i < N-1 )
    du=

      (u[-1+i]-u[i])/(x[-1+i]-x[i])+
      (-u[-1+i]+u[1+i])/(x[-1+i]-x[1+i])+
      (u[i]-u[1+i])/(x[i]-x[1+i]);
  else if( i == 0 )
    du=


      (u[1]-u[0])/(x[1]-x[0])
      +(u[2]-u[0])/(x[2]-x[0])
      -(u[2]-u[1])/(x[2]-x[1]);
  else if( i == N-1 )
    du=
      (-u[-3+N]+u[-2+N])/(x[-3+N]-x[-2+N])
      +(u[-3+N]-u[-1+N])/(x[-3+N]-x[-1+N])
      +(u[-2+N]-u[-1+N])/(x[-2+N]-x[-1+N]);
  else du = 0.;

  assert(du-du==0.);

  /* printf("D1: i=%i du=%.15f\n", i, du); */
  return du;
}

/* obliczanie drugiej pochodnej */
double _D2 ( double * u, double * x, int i, int N )
{
  double ddu;

  if( i > 0 && i < N-1 )
    ddu =
      (2*((u[-1+i]-u[1+i])/(x[-1+i]-x[1+i])+(-u[i]+u[1+i])/(x[i]-x[1+i])))/(x[-1+i]-x[i]);

/* ((u[i+1]-u[i])/(x[i+1]-x[i]) */
/* 	    -(u[i]-u[i-1])/(x[i]-x[i-1])) */
/*       *2./(x[i+1]-x[i-1]); */
  else if (i == 0)
    ddu = (2*((u[i]-u[2+i])/(x[i]-x[2+i])+(-u[1+i]+u[2+i])/(x[1+i]-x[2+i])))/(x[i]-x[1+i]);
  /* ddu =  (2.* */
    /* 	    ( (u[i]-u[2+i])/(x[i]-x[2+i]) */
    /* 	      +(-u[1+i]+u[2+i])/(x[1+i]-x[2+i])) */
    /* 	    )/(x[i]-x[1+i]); */
  else if (i==N-1)
    ddu =  (2.*
	    ( (u[-2+i]-u[i])/(x[-2+i]-x[i])
	      +(-u[-1+i]+u[i])/(x[-1+i]-x[i])
	      )
	    )/(x[-2+i]-x[-1+i]);
  else ddu = 0.;

  assert(ddu-ddu==0.);

  return ddu;

}

/* funkcja definiujaca transformacje Sundmana
   dt/dtau=g(u)=0.01/(du/dx(0,tau))^2 */
double _g ( double * y, int N )
{
  double d1,d2;
  d1=_D1(y+1,y+1+N,1,N);
  d2=_D1(y+1,y+1+N,N-2,N);
  d1=d1>d2?d1:d2;

  return
    0.01
    *pow(fabs(d1/sin(y[N+2])),-2);
}

/* funkcja rozkladu punktow fizycznej siatki (
   M(x)=du/dx+sqrt(d2u/dx2) ) */
void _M_calc ( double * ui, double * xi, double * M, int N )
{
  int i;
  double Mtot=0.,du,ddu,x,u;

  for ( i = 1; i < N-1; i++)
    {
      x=xi[i];
      du=_D1( ui, xi, i, N );
      ddu=_D2( ui, xi, i, N );
      u=ui[i];

      M[i]=fabs( du-u/tan(x) )/sin(x)
	+ sqrt( fabs(
		     ddu
		     -2./tan(x)*du
		     +(pow(cos(x),2)+1)*u/pow(sin(x),2)
		     )
		/sin(x)
		);
      Mtot+=(M[i]*(xi[i+1]-xi[i-1])/2.);

      /* printf("M_calc: i=%i, x=%f, sin(x)=%f, M[i]=%.15f\n", */
      /* 	     i, x, sin(x), M[i]); */
      /* printf("M_calc: M[i]-M[i]=%.15f\n", M[i]-M[i]); */

      assert( M[i]-M[i]==0. );
      assert( M[i] >= 0 );
    }

  M[0]=fabs( _D2( ui, xi, 0, N ) );

  M[N-1]=fabs( _D2( ui, xi, N-1, N ) );

  Mtot+=M[0]*(xi[1]-xi[0])+M[N-1]*(xi[N-1]-xi[N-2]);

  for ( i = 0; i < N; i++ )
    {
      M[i]/=Mtot;
      /* M[i]+=.01*(1.+sin(x[i])); */
    }
  /* printf("----M[0]=%f Mtot=%f\n",M[0],Mtot); */
}

void plot_init ( void * solver, void * module )
{
/*   ODE_module * m = (ODE_module*)module; */
/*   plot_module_data * data = (plot_module_data*)m->data; */

/*   data->plotter = gnuplot_init(); */
/*   gnuplot_setstyle( data->plotter, "lines" ); */
}

void plot_step ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  ODE_solver * s = (ODE_solver*)solver;
  plot_module_data * data = (plot_module_data*)m->data;
  int N = s->params->Nx;

  gnuplot_ctrl * plotter = data->plotter;
  char * title = data->title;
  H_DOUBLE * t = s->state->t;
  int i;
  double x;
  double x1 = s->params->basis->params->x1;


  N=(N-1)/2;

  for(i = 1; i < N-1; i++)
    {
      x=s->state->f[1+N+i];
      s->params->Dtemp[0][0][i]=s->state->f[i+1]/* /x */;
      s->params->Dtemp[0][1][i]=x;
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
  		  s->state->f/* +1+N */,
  		  N*2+1,
  		  title );

}

void plot_stop ( void * solver, void * module )
{
  /* ODE_module * m = (ODE_module*)module; */
  /* plot_module_data * data = (plot_module_data*)m->data; */

/*   gnuplot_close( data->plotter ); */
}

void plot_data_free ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  plot_module_data * data = (plot_module_data*)m->data;

  gnuplot_close( data->plotter );
  free( m->data );
}

ODE_module * ODE_module_plot_init ( H_DOUBLE dt )
{
  ODE_module * plot_module = malloc ( sizeof ( ODE_module ) );
  plot_module_data * data = malloc( sizeof(plot_module_data) );

  data->plotter = gnuplot_init();
  gnuplot_setstyle( data->plotter, "linespoints" );
  /* gnuplot_cmd( data->plotter, "set xrange [0:pi]\n" ); */

  plot_module->data = data;

  plot_module->init =
    &plot_init;

  plot_module->step =
    &plot_step;

  plot_module->stop =
    &plot_stop;

  plot_module->data_free =
    &plot_data_free;

  plot_module->dt = dt;
  plot_module->t_last = 0.;

  return plot_module;
}
