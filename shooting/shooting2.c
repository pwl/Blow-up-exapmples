#include <stdio.h>
#include "math.h"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

# define M_PI		3.14159265358979323846	/* pi */


int
func (double t, const double y[], double f[],
      void *params)
{
  double k = *(double *)params;
  f[0] = y[1];
  f[1] = (k-1)/2.*sin(2.*y[0])/t/t-((k-1.)/t+t/2.)*y[1];
  return GSL_SUCCESS;
}

int
jac (double t, const double y[], double *dfdy,
     double dfdt[], void *params)
{
  double k = *(double *)params;
  gsl_matrix_view dfdy_mat
    = gsl_matrix_view_array (dfdy, 2, 2);
  gsl_matrix * m = &dfdy_mat.matrix;
  gsl_matrix_set (m, 0, 0, 0.0);
  gsl_matrix_set (m, 0, 1, 1.0);
  gsl_matrix_set (m, 1, 0, (k-1)/t/t*cos(2.*y[0]));
  gsl_matrix_set (m, 1, 1, (k-1)/t+t/2.);
  dfdt[0] = 0.0;
  dfdt[1] = 0.0;
  return GSL_SUCCESS;
}

double
fevol (double L, double A, int print)
{
  const gsl_odeiv_step_type * T
    = gsl_odeiv_step_bsimp;

  gsl_odeiv_step * s
    = gsl_odeiv_step_alloc (T, 2);
  gsl_odeiv_control * c
    = gsl_odeiv_control_y_new (1e-5, 0.0);
  gsl_odeiv_evolve * e
    = gsl_odeiv_evolve_alloc (2);

  double k = 3.;
  double dt=.0001, t_last=0.;
  int i = 0;

  char name[100];

  gsl_odeiv_system sys = {func, jac, 2, &k};

  double t0 = 1.e-5, t1 = L, t = t0;
  double h = 1e-5;
  double y[2] = {
    M_PI + A*t0 - (A*(3 + 8*pow(A,2))*pow(t0,3))/60. +
    (A*(3 + 16*pow(A,2) + 32*pow(A,4))*pow(t0,5))/1120.,
    A - (A*(3 + 8*pow (A, 2))*pow (t0, 2))/
    20. + (A*(3 + 16*pow (A, 2) + 32*pow (A, 4))*pow (t0, 4))/
    224.};

  FILE * file;

  if (print)
    {
      sprintf(name, "bisection.dat");
      file = fopen(name, "w");
    }

  while (t < t1)
    {
      int status =
	gsl_odeiv_evolve_apply (e, c, s,
				&sys,
				&t, t1,
				&h, y);

      if (status != GSL_SUCCESS)
	break;

      i++;
      if (print && t_last+dt < t)
	{
	  fprintf (file,
		   "%.15f %.15f %.15f\n",
		   t, y[0], y[1]);
	  printf ("%.15f %.15f %.15f\n",
		   t, y[0], y[1]);
	  t_last+=dt;
	  dt*=4.;
	}
    }

  gsl_odeiv_evolve_free (e);
  gsl_odeiv_control_free (c);
  gsl_odeiv_step_free (s);

  if (print)
    fclose( file );

  return y[0];
}

double
bisec(double L, double A0, double A1, double e)
{
  double f;
  int i=1;

  while(A1-A0 > e)
    {
      printf("%03i, A=%.20f\n",i++,.5*(A0+A1));
      f=fevol(L,-.5*(A0+A1),0);
      if( (f-2.18)>0. )
	A0=.5*(A0+A1);
      else
	A1=.5*(A0+A1);
    }
  return A0;
}

int
main (void)
{
  /* double A, fA; */
  FILE * f;

  f=fopen ("test2.dat","w");

  /* double b = bisec ( 1.e3, .0, 5., 1.e-15 ); */
  /* printf( "bisect=%.15f\n", b); */

  /* for( A=1e-5; A<1e4; A*=1.2 ) */
  /*   { */
  /*     fA=fevol(100.,-A,0); */
  /*     printf("%f %f\n",A,fA); */
  /*     fprintf(f,"%.15f %.15f\n",A,fA); */
  /*   } */

  /* fevol ( 1e3, -b, 1 ); */
  /* fevol ( 100., -1., 1 ); */
  /* fevol ( 100., -10., 1 ); */
  /* fevol ( 100., -100., 1 ); */
  /* fevol ( 100., -1000., 1 ); */
  /* fevol ( 100., -10000., 1 ); */
  fevol ( 1000., -0.45639253339850915925, 1 );
  fclose(f);
  return 0;
}
