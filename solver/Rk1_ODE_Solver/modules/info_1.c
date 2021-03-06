#include "info_1.h"

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
  int N = (s->params->Nx-1)/2, i;
  H_DOUBLE tau = *(s->state->t);
  H_DOUBLE * f = s->state->f;
  H_DOUBLE * df = s->state->df;
  H_DOUBLE * dtau = s->state->dt;
  H_DOUBLE * u = f+1;
  H_DOUBLE * x = f+1+N;
  H_DOUBLE g = (s->state->df[0]);
  H_DOUBLE t = f[0];
  H_DOUBLE dx = D1_at_0(u,x);/* D1(u,x,0,N); */
  H_DOUBLE ddx = D2(u,x,0,N);
  H_DOUBLE dddx = D3_at_0(u,x);
  H_DOUBLE min = 1.;
  H_DOUBLE d2fdtdx = D1(df+1,x,0,N);
  FILE * file;



  file = fopen ( data->fileNames[0], "a" );
  fprintf( file, "%.20G %.20G %.20G %.20G %.20G %.20G %.20G %.20G %.20G %.20G %.20G\n",
  	   tau, t, u[1], x[1], dx, g, *dtau, df[2], d2fdtdx, ddx, dddx );
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

double D3_at_0 (double * u, double * x)
{
  return 6*u[1]*(x[4]*x[5] + x[3]*(x[4] + x[5]) + x[2]*(x[3] + x[4] + x[5]))*pow(x[1],-1)*pow(x[1] - x[2],-1)*
    pow(x[1] - x[3],-1)*pow(x[1] - x[4],-1)*pow(x[1] - x[5],-1) -
   6*u[2]*(x[4]*x[5] + x[3]*(x[4] + x[5]) + x[1]*(x[3] + x[4] + x[5]))*pow(x[1] - x[2],-1)*pow(x[2],-1)*
    pow(x[2] - x[3],-1)*pow(x[2] - x[4],-1)*pow(x[2] - x[5],-1) +
   6*u[3]*(x[4]*x[5] + x[2]*(x[4] + x[5]) + x[1]*(x[2] + x[4] + x[5]))*pow(x[1] - x[3],-1)*
    pow(x[2] - x[3],-1)*pow(x[3],-1)*pow(x[3] - x[4],-1)*pow(x[3] - x[5],-1) -
   6*u[4]*(x[3]*x[5] + x[2]*(x[3] + x[5]) + x[1]*(x[2] + x[3] + x[5]))*pow(x[3] - x[4],-1)*pow(x[4],-1)*
    pow(-x[1] + x[4],-1)*pow(-x[2] + x[4],-1)*pow(x[4] - x[5],-1) -
   6*u[5]*(x[3]*x[4] + x[2]*(x[3] + x[4]) + x[1]*(x[2] + x[3] + x[4]))*pow(x[3] - x[5],-1)*pow(x[5],-1)*
    pow(-x[1] + x[5],-1)*pow(-x[2] + x[5],-1)*pow(-x[4] + x[5],-1);
}

double D1_at_0 (double * u, double * x)
{
  return -(u[1]*x[2]*x[3]*x[4]*pow(x[1],-1)*pow(x[1] - x[2],-1)*pow(x[1] - x[3],-1)*pow(x[1] - x[4],-1)) +
   u[2]*x[1]*x[3]*x[4]*pow(x[1] - x[2],-1)*pow(x[2],-1)*pow(x[2] - x[3],-1)*pow(x[2] - x[4],-1) +
   u[3]*x[1]*x[2]*x[4]*pow(x[2] - x[3],-1)*pow(x[3],-1)*pow(-x[1] + x[3],-1)*pow(x[3] - x[4],-1) +
    u[4]*x[1]*x[2]*x[3]*pow(x[2] - x[4],-1)*pow(x[4],-1)*pow(-x[1] + x[4],-1)*pow(-x[3] + x[4],-1);
}
