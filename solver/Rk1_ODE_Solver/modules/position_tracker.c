#include "position_tracker.h"

void position_tracker_init ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  position_tracker_module_data * data =
    (position_tracker_module_data*)m->data;
  char * file_name = data->out_file;
  

  /* clear the file before writing */
  fclose( fopen( file_name, "w" ) );
}

void position_tracker_step ( void * solver, void * module )
{
  ODE_module * m = (ODE_module*)module;
  ODE_solver * s = (ODE_solver*)solver;
  position_tracker_module_data * data =
    (position_tracker_module_data*)m->data;
  char * file_name = data->out_file;
  FILE * file;
  int i, N=s->params->Nx;
  H_DOUBLE x = 0., a = data->a,
    t_last = data->time_last, p_last = data->pos_last,
    * xi = s->params->basis->collocation_points,
    * f = s->state->f,
    A,B;

  
  for ( i = 0; i < N-1; i++ )
    if ( (f[i]-a)*(f[i+1]-a) < 0 )
      {
	A=(f[i+1]-f[i])/(xi[i+1]-xi[i]);
	B=(f[i]*xi[i+1]-f[i+1]*xi[i])/(xi[i+1]-xi[i]);
	x=(a-B)/A;
      }
  

  if ( x == 0. )
    return;
  
  file = fopen( file_name, "a");
  fprintf( file, "%.20f %.20f %.20f\n",
	   *s->state->t, x, (x - p_last)/(*s->state->t - t_last));
  fclose( file );

  data->pos_last = x;
  data->time_last = *s->state->t;
  
}

void position_tracker_stop ( void * solver, void * module )
{
}


ODE_module * ODE_module_position_tracker_init ( H_DOUBLE dt, H_DOUBLE a, H_DOUBLE da, char out_file[] )
{
  ODE_module * position_tracker_module = malloc ( sizeof ( ODE_module ) );
  position_tracker_module_data * data = malloc( sizeof(position_tracker_module_data) );

  strcpy ( data->out_file, out_file );
  data->a = a;
  data->da = da;
  data->pos_last = 0.;
  data->time_last = 0.;
  
  position_tracker_module->data =
    data;

  position_tracker_module->init = 
    &position_tracker_init;
  
  position_tracker_module->step =
    &position_tracker_step;
  
  position_tracker_module->stop = 
    &position_tracker_stop;
  
  position_tracker_module->data_free = 
    NULL;
 
  position_tracker_module->t_last=0.;
  position_tracker_module->dt = dt;
  
  return position_tracker_module;
}
