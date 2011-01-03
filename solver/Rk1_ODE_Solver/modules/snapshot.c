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
  H_DOUBLE * ui = f+1;
  H_DOUBLE * xi = f+1+N;
  H_DOUBLE u,x,du,ddu,dudt;
  char temp_c[100];
  H_DOUBLE t = *s->state->t;
  H_DOUBLE * df = s->state->df;
  H_DOUBLE d2udtdr0=D1(df+1,f+N+1,0,N)/df[0];
  H_DOUBLE dudr0=D1(f+1,f+N+1,0,N);
  H_DOUBLE g = df[0]/0.02;	/* g =.5*dudr/d2udtdr0 */
  /* H_DOUBLE norm = sqrt(exps)/(dudr0-2./exps*d2udtdr0); */

  sprintf( temp_c, "log/snapshot/snap_%.15f.dat", f[0] );

  file=fopen( temp_c ,"w");

  fprintf(file,"# t = %.15G\n",f[0]);
  fprintf(file,"# s = %.15G\n",t);
  fprintf(file,"# N = %.2f\n",k);
  fprintf(file,"# du = %.15G\n",
	  D1( ui, xi, 0, N ) );
  fprintf(file,"# g = %.15G\n", g );

  /* fprintf(file,"%.15G %.15G %.15G %.15G\n", */
  /*    0., 0., D1(ui,xi,0,N), 0., g); */

  fprintf(file,"%.15G %.15G %.15G %.15G\n",
  	  0., D1(ui,xi,0,N), D1(ui,xi,0,N), 0., g);


  for( i=1; i<N-1; i++)
    {
      x=xi[i];
      u=ui[i];
      /* obliczenie pochodnych w punkcie "i" */
      du=0.;/* D1(ui,xi,i,N); */
      ddu=D2(ui,xi,i,N);
      /* dudt=ddu+(k-1.)/x*du-(k-1.)/2.*sin(2.*u)/x/x; */
      /* dudt=ddu-sin(2.*u/x)/x; */
      dudt=0.;
      /* fprintf(f,"%f %f\n",s->params->basis->collocation_points[i],s->state->f[i]); */
      fprintf
	(file,"%.15G %.15G %.15G %.15G %.15G\n",
	 x, u/sin(x), du, dudt, g
	 /* (xi[i]*D1(ui,xi,i,N)-2./exps*dudt)*norm */
	 );
    }

  (file,"%.15G %.15G %.15G %.15G\n",
     PI, -D1(ui,xi,N-1,N), D1(ui,xi,0,N), 0., g);


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
