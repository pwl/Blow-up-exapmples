#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
#include <math.h>

int
func (double t, const double y[], double f[],
      void *params)
{
  double k = *(double *)params;
  f[0] = y[1];
  f[1] = sin(2.*y[0])/t/t-(2./t-t/2.)*y[1];
  /* f[2] = y[3]; */
  /* f[3] = (2.*cos(2.*y[0])/t/t+k)*y[2]-(2./t/\* -t/2. *\/)*y[3]; */
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
  gsl_matrix_set (m, 1, 1, (k-1)/t-t/2.);
  dfdt[0] = 0.0;
  dfdt[1] = 0.0;
  return GSL_SUCCESS;
}

double
fevol (double L, double B, int print)
{
 const gsl_odeiv_step_type * T
    = gsl_odeiv_step_rk8pd;

  gsl_odeiv_step * s
    = gsl_odeiv_step_alloc (T, 2);
  gsl_odeiv_control * c
    = gsl_odeiv_control_y_new (1e-15, 0.0);
  gsl_odeiv_evolve * e
    = gsl_odeiv_evolve_alloc (2);

  double k = B;

  double dt=0.01, t_last=0.;

  gsl_odeiv_system sys = {func, jac, 2, &k};

  double t0 = 1.e-13, t1 = L, t = t0;
  double h = 1e-14;
  double A = B;
  double y[2] = {
    A*t0 + (pow (t0, 3)*(3*A - 8*pow (A, 3)))/60. +
    (A*pow (t0, 5)*(3 - 16*pow (A, 2) + 32*pow (A, 4)))/1120.,
    A + (pow (t0, 2)*(3*A - 8*pow (A, 3)))/20.
    + (A*pow (t0, 4)*(3 - 16*pow (A, 2) + 32*pow (A, 4)))/224.

    /* A*t0, */
    /* A */
  };

  FILE * file;
  if (print) file = fopen("bisection.dat", "w");

  while (t < t1)
    {
      int status =
	gsl_odeiv_evolve_apply (e, c, s,
				&sys,
				&t, t1,
				&h, y);

      if (status != GSL_SUCCESS)
	break;
      if ( fabs(y[0]-2.18) > 3. ||
	   fabs(y[2]) > 10. )
	break;

      if (print && t_last+dt < t)
	{
	  fprintf (file,
		   "%.15f %.15f %.15f\n",
		   t, y[0], y[1]/* , y[2], y[3] */);
	  t_last+=dt;
	}
      printf("%.15f\r",t);
    }

  gsl_odeiv_evolve_free (e);
  gsl_odeiv_control_free (c);
  gsl_odeiv_step_free (s);
  if (print) fclose( file );

  return y[2];
}

double
bisec(double L, double A0, double A1, double e)
{
  double f;
  int i=1;

  while(fabs(A1-A0) > e)
    {
      printf("%03i, A=%.20f\n",i++,.5*(A0+A1));
      f=fevol(L,.5*(A0+A1),0);
      if( f > 0 )
	A0=.5*(A0+A1);
      else
	A1=.5*(A0+A1);
    }
  return A0;
}

int
main (void)
{
  double x;
  FILE * f;

  /* f=fopen ("test1.dat","w"); */

  /* double b = bisec ( 100., -4., -2., 1.e-15 ); */
  /* printf( "bisect=%.15f\n", b); */

  for( x=-100.; x<1.; x+=.1 )
    {
      printf("%.15f\n",x);
      fprintf(f,"%.15f %.15f\n",x,fevol(100.,x,0));
    }

  /* fevol ( 100., b, 1 ); */
  /* fevol ( 100000., 1., 1 ); */
  /* fevol ( 100./.01, 2.7387531258849548976*.01, 1 ); */
  /* fevol ( 100., 2.7387531258849548976, 1 ); */
  /* fclose(f); */
  return 0;
}
