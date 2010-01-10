#include "snapshot.h"

void snapshot_init ( void * solver, void * module )
{
}

void snapshot_step ( void * solver, void * module )
{
  /* ODE_module * m = (ODE_module*)module; */
  ODE_solver * s = (ODE_solver*)solver;
  FILE * f;
  char temp_c[100];
  H_DOUBLE * t = s->state->t;
  int i,N=s->params->Nx;
  N=(N-1)/2;

  sprintf( temp_c, "log/snapshot/snap_%.15f.dat", s->state->f[0] );

  f=fopen( temp_c ,"w");

  fprintf(f,"# t = %.15E\n",s->state->f[0]);

  for( i=0; i<N; i++)
    {
      /* fprintf(f,"%f %f\n",s->params->basis->collocation_points[i],s->state->f[i]); */
      fprintf(f,"%.15E %.15E\n",s->state->f[i+N+1],s->state->f[i+1]);
    }

  fclose(f);
}

void snapshot_stop ( void * solver, void * module )
{
}

ODE_module * ODE_module_snapshot_init ( H_DOUBLE dt )
{
  ODE_module * snapshot_module = malloc ( sizeof ( ODE_module ) );

  snapshot_module->init =
    &snapshot_init;

  snapshot_module->step =
    &snapshot_step;

  snapshot_module->stop =
    &snapshot_stop;

  snapshot_module->data_free =
    NULL;

  snapshot_module->dt = dt;
  snapshot_module->t_last = 0.;

  return snapshot_module;
}
