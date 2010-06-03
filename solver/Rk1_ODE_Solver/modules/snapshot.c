#include "snapshot.h"

void snapshot_init ( void * solver, void * module )
{
}

void snapshot_step ( void * solver, void * module )
{
  ODE_solver * s = (ODE_solver*)solver;
  FILE * file;
  int i,N = (s->params->Nx-1)/2;
  H_DOUBLE * f = s->state->f;
  H_DOUBLE * u = f+1;
  H_DOUBLE * x = f+1+N;
  char temp_c[100];
  H_DOUBLE t = *s->state->t;

  sprintf( temp_c, "log/snapshot/snap_%.15f.dat", f[0] );

  file=fopen( temp_c ,"w");

  fprintf(file,"# t = %.15G\n",f[0]);
  fprintf(file,"# s = %.15G\n",t);
  fprintf(file,"# N = %.2f\n",k);
  fprintf(file,"# du = %.15G\n",
	  D1( u, x, 0, N ) );
  fprintf(file,"# g = %.15G\n",
	  s->state->df[0] );

  for( i=0; i<N; i++)
    {
      /* fprintf(f,"%f %f\n",s->params->basis->collocation_points[i],s->state->f[i]); */
      fprintf
	(file,"%.15G %.15G %.15G %.15G\n",
	 x[i], u[i], D1(u,x,i,N), s->state->df[1+i]/(s->state->df[N-1]/(x[N-2]-PI/2.)));
    }

  fclose(file);
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
