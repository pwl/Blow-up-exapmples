#include "info_1.h"

double DD1 ( double * u, double * x, int i, int N )
{
  if( i > 0 && i < N-1 )
    return
      (u[-1+i]-u[i])/(x[-1+i]-x[i])+
      (-u[-1+i]+u[1+i])/(x[-1+i]-x[1+i])+
      (u[i]-u[1+i])/(x[i]-x[1+i]);
  else if( i == 0 )
    return
      (u[1]-u[0])/(x[1]-x[0])
      +(u[2]-u[0])/(x[2]-x[0])
      -(u[2]-u[1])/(x[2]-x[1]);
  else if( i == N-1 )
    return
      (-u[-3+N]+u[-2+N])/(x[-3+N]-x[-2+N])
      +(u[-3+N]-u[-1+N])/(x[-3+N]-x[-1+N])
      +(u[-2+N]-u[-1+N])/(x[-2+N]-x[-1+N]);
    else return 0.;
} /* delete this later */


void info_1_init ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  ODE_solver * s = (ODE_solver*)solver;
  int N = s->params->Nx;
  int i;
  info_1_module_data * data = (info_1_module_data*)m->data;
  int Npt = data->Npt;
  FILE * file;

  i=0;

  file = fopen( data->fileNames[i], "w");
  fclose( file );
  data->pt[i] = i ;
}

void info_1_step ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  ODE_solver * s = (ODE_solver*)solver;
  info_1_module_data * data = (info_1_module_data*)m->data;
  int N = (s->params->Nx-1)/2;
  H_DOUBLE tau = *(s->state->t);
  H_DOUBLE * f = s->state->f;
  H_DOUBLE * df = s->state->df;
  H_DOUBLE * dtau = s->state->dt;
  H_DOUBLE * u = f+1;
  H_DOUBLE * x = f+1+N;
  H_DOUBLE g = (s->state->df[0]);
  H_DOUBLE t = f[0];
  H_DOUBLE dx;
  FILE * file;

  dx=DD1(u,x,0,N);

  file = fopen ( data->fileNames[0], "a" );
  fprintf( file, "%.15E %.15E %.15E %.15E %.15E %.15E %.15E %.15E\n",
  	   tau, t, u[1], x[1], dx, g, *dtau, df[2] );
  fclose( file );
}

void info_1_stop ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  /* info_1_module_data * data = (info_1_module_data*)m->data; */

}

void info_1_data_free ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  info_1_module_data * data = (info_1_module_data*)m->data;
  int i, N = data->Npt;

  for (i = 0; i < N; i++)
    {
      free (data->fileNames[i]);
    }

  free( data->pt );
  free( data->fileNames );
  free( data );
}

ODE_module * ODE_module_info_1_init( H_DOUBLE dt, int N )
{
  ODE_module * m = malloc ( sizeof ( ODE_module ) );
  info_1_module_data * data =
    malloc( sizeof(info_1_module_data) );
  int i;

  m->data = data;
  data->pt = malloc( N * sizeof( int ) );
  data->fileNames = malloc( N * sizeof( char * ) );

  for (i = 0; i < N; i++)
    {
      data->fileNames[i] = malloc( 100 * sizeof(char));
      sprintf(data->fileNames[i], "log/info_1/log%03i.dat", i );
    }

  data->Npt = N;

  m->init =
    &info_1_init;

  m->step =
    &info_1_step;

  m->stop =
    &info_1_stop;

  m->data_free =
    &info_1_data_free;

  /* negative value means not to execute step function */
  m->dt = dt;
  m->t_last = .0;
  sprintf(m->name, "info_1");

  return m;
}
