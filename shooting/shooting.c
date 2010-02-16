#include "shooting.h"

void
solve_shrinker_eigenproblem
(double A,
 int index)
{
  FILE *eigenfile;
  int j, eigen_results_collected;
  double eigen_results[100];
  char eigenfile_name[100];

  printf(GREEN1 "solving eigenproblem for index %i in dimension %.3f with l=%.1f\n" FORMAT_OFF,
	 index, k, l);

  eigen_results_collected = harvester
    ( 20.,
      2.,
      1.,
      RIPPER_LINEAR,
      10,
      eigen_results,
      0.,
      fevol_shrinker_eigenproblem,
      (void*)(&A) );

  sprintf
    ( eigenfile_name,
      HARVESTER_DATA_DIR "eigen" HARVESTER_DEFAULT_EIGEN_EXTENSION,
      k, l, index);

  eigenfile = fopen( eigenfile_name, "w" );

  for ( j = 0; j < eigen_results_collected; j++ )
    {
      fprintf
	(eigenfile,
	 "%.15f %.1f %i %.15f %i %.15f\n",
	 k, l, index, A, j+1, eigen_results[j] );
    }

  fclose( eigenfile );
}


void print_shrinker_profile( double A ) {
  char shrinkerfile_name[50];
  FILE *shrinkerfile;

  sprintf
    ( shrinkerfile_name,
      HARVESTER_DATA_DIR "shrinker" HARVESTER_DEFAULT_EXTENSION,
      k, l);
  shrinkerfile = fopen( shrinkerfile_name, "a" );
  fprintf( shrinkerfile, "# k = %.15f\n# l = %.1f\n", k, l );
  fclose( shrinkerfile );
  fevol_shrinker( A, 1 , shrinkerfile_name , NULL );
}

double
bisec(double A0,
      double A1,
      double e,
      double val,
      double (*fevol)(double, int, char *, void *),
      void * param)
{
  double f,f0,f1;
  int i=1;
  double B;

  /* swap the values if they are not sorted */
  if ( A0 > A1 )
    {
      B = A0;
      A0 = A1;
      A1 = B;
    }

  f0 = fevol( A0, 0, "",param) - val;
  f1 = fevol( A1, 0, "",param) - val;

  while( 2.*(A1-A0)/fabs(A1+A0) > e) /* relative error measure */
    {
      printf( "%03i, A=%.20f, delta/A=%.1E\r",
	      i++, .5*(A0+A1), 2.*(A1-A0)/(A0+A1) );

      f = fevol(.5*(A0+A1),0, "", param) - val;

      if( f*f0 > 0 )
	A0=.5*(A0+A1);
      else
	A1=.5*(A0+A1);
    }
  printf("\n");

  return .5*(A0+A1);
}

int
harvester(
	  double range_min,
	  double range_max,
	  double delta,
	  unsigned int opt,
	  int results_max,
	  double * results ,
	  double val,
	  double (*fevol)(double, int, char *, void *),
	  void * param)
{

  double cursor = range_min;
  double cursor_last = cursor;
  double b=range_max,a=range_min;
  double value;
  double value_last;
  double epsilon = RIPPER_BISEC_EPSILON;
  double t=0.,s=0.,dt;			/* t in (0,1] */
  int results_collected = 0;


  value_last = fevol( cursor, 0, "", param ) - val;

  switch( opt )
    {
    case RIPPER_LINEAR:
      dt = delta/fabs(b-a);
      break;
    case RIPPER_DENSE1:
      dt = sqrt(1.-pow(1.-delta/fabs(b-a),2));
      break;
    default:
      dt = delta/(b-a);
    }

  /* printf("cursor_last = %.5E, cursor = %.5E, dt = %.5E, s = %.5E\n", cursor_last, cursor, dt, s); */

  while( s < 1. )
    {
      t+=dt;
      switch( opt ) {
      case RIPPER_LINEAR:
	s=t;
	break;
      case RIPPER_DENSE1:
	s=1.-sqrt(1.-t*t);
	break;
      default:
	s=t;
      }

      cursor_last = cursor;
      cursor = a+s*(b-a);
      value = fevol( cursor, 0, "", param ) - val;

      printf("cursor = %.10E\r", cursor);

      /* printf("cursor_last = %.5E, cursor = %.5E, dt = %.5E, s = %.5E\n", cursor_last, cursor, dt, s); */

      /* if signs are opposite */
      if( value_last*value < 0 )
      	{
	  results[results_collected] =
	    bisec( cursor_last, cursor, epsilon, val, fevol, param );

	  results_collected++;
	  printf(CYAN1 "collected result " BOLD1 "%.15f" FORMAT_OFF CYAN1 ", %i results in total\n" FORMAT_OFF,
		 results[results_collected-1], results_collected);

	  if( results_collected >= results_max )
	    return results_collected;

	  value_last = value;
	}
    }

  return results_collected;
}



int
jac_dummy (double t, const double y[], double *dfdy,
	   double dfdt[], void *params)
{
  return -1;
}

int
func_shrinker (double t, const double y[], double f[],
	       void *params)
{
  f[0] = y[1];
  f[1] = sin(2.*y[0])*l*(l+k-2.)/2./t/t-((k-1.)/t-t/2.)*y[1];
  return GSL_SUCCESS;
}

double
fevol_shrinker (double bisec_param, int print, char * filename, void * p)
{
  const gsl_odeiv_step_type * T
    = STEPPER;

  FILE * file = NULL;

  gsl_odeiv_step * s
    = gsl_odeiv_step_alloc (T, 2);
  gsl_odeiv_control * c
    = gsl_odeiv_control_y_new (STEPPER_ERROR, 0.0);
  gsl_odeiv_evolve * e
    = gsl_odeiv_evolve_alloc (2);

  double dt=PRINT_DT, t_last=0.;

  gsl_odeiv_system sys = {func_shrinker, jac_dummy, 2, p};

  double t = T0;
  double h = H0;
  double A = bisec_param;
  double y[2] = {
    (A + A*l*pow (8*l + 4*k, -1)*
     pow (t, 2) + (A*l*(2 + l)*pow (2*l + k, -1)*pow (2 + 2*l + k, -1)*
		   pow (t, 4))/
     32. + (A*l*(2 + l)*(4 + l)*pow (2*l + k, -1)*
	    pow (2 + 2*l + k, -1)*pow (4 + 2*l + k, -1)*pow (t, 6))/
     384.)*pow (t, l),

    (A*l*pow (2*l + k, -1)*pow (2 + 2*l + k, -1)*
     pow (4 + 2*l + k, -1)*(384*(2*l + k)*(2 + 2*l + k)*(4 + 2*l + k) +
			    96*(2 + l)*(2 + 2*l + k)*(4 + 2*l + k)*pow (t, 2) +
			    12*(2 + l)*(4 + l)*(4 + 2*l + k)*
			    pow (t, 4) + (2 + l)*(4 + l)*(6 + l)*pow (t, 6))*
     pow (t, -1 + l))/384.

    /* A*t + ((3*A - 4*(-1 + k)*pow(A,3))*pow(2 + k,-1)*pow(t,3))/12. + */
    /* (A*(15 - 40*(-1 + k)*pow(A,2) + 16*pow(A,4)*(1 - 3*k + 2*pow(k,2)))* */
    /*  pow(t,5)*pow(8 + 6*k + pow(k,2),-1))/160., */
    /* A + ((3*A - 4*(-1 + k)*pow(A,3))*pow(2 + k,-1)*pow(t,2))/4. + */
    /* (A*(15 - 40*(-1 + k)*pow(A,2) + 16*pow(A,4)*(1 - 3*k + 2*pow(k,2)))* */
    /*  pow(t,4)*pow(8 + 6*k + pow(k,2),-1))/32., */
  };


  if (print){
    file = fopen(filename, "a");
    fprintf(file, "# A = %.15f\n", bisec_param );
  }

  while (t < T_MAX)
    {
      int status =
	gsl_odeiv_evolve_apply (e, c, s,
				&sys,
				&t, T_MAX,
				&h, y);

      if (status != GSL_SUCCESS)
	break;
      /* are we still in the strip [0,pi]? */
      if ( 0. > y[0] || y[0] > 3.15 )
	break;

      if (print /* && t_last+dt < t */)
	{
	  fprintf (file,
		   "%.15f %.15f %.15f\n",
		   t, y[0], y[1]/* , y[2], y[3] */);
	  t_last+=dt;
	  dt*=PRINT_DT_RATIO;
	}
      /* printf("%.15f\r",t); */
    }

  gsl_odeiv_evolve_free (e);
  gsl_odeiv_control_free (c);
  gsl_odeiv_step_free (s);

  if (print) {
    fprintf( file, "\n\n\n" );
    fclose( file );
  }

  return y[1];
}

int
func_shrinker_eigenproblem (double t, const double y[], double f[],
			    void *params)
{
  double lambda = *(double*)params;
  f[0] = y[1];
  f[1] = l*(l+k-2.)/2.*sin(2.*y[0])/t/t-((k-1.)/t-t/2.)*y[1];
  f[2] = y[3];
  f[3] = (l*(l+k-2.)*cos(2.*y[0])/t/t+lambda)*y[2]-((k-1.)/t-t/2.)*y[3];
  return GSL_SUCCESS;
}

double
fevol_shrinker_eigenproblem (double bisec_param, int print, char * filename, void * p)
{
  const gsl_odeiv_step_type * T
    = STEPPER;

  FILE * file = NULL;

  gsl_odeiv_step * s
    = gsl_odeiv_step_alloc (T, 4);
  gsl_odeiv_control * c
    = gsl_odeiv_control_y_new (STEPPER_ERROR, 0.0);
  gsl_odeiv_evolve * e
    = gsl_odeiv_evolve_alloc (4);

  double dt=PRINT_DT, t_last=0.;

  gsl_odeiv_system sys = {func_shrinker_eigenproblem, jac_dummy, 4, (void*)&bisec_param};

  double t = T0;
  double h = H0;
  double A = *(double*)p;
  double y[4] = {		      /* expressions derived using
					 ~/SeriesSolve.nb */
    (A + A*l*pow (8*l + 4*k, -1)*
     pow (t, 2) + (A*l*(2 + l)*pow (2*l + k, -1)*pow (2 + 2*l + k, -1)*
		   pow (t, 4))/
     32. + (A*l*(2 + l)*(4 + l)*pow (2*l + k, -1)*
	    pow (2 + 2*l + k, -1)*pow (4 + 2*l + k, -1)*pow (t, 6))/
     384.)*pow (t, l),

    (A*l*pow (2*l + k, -1)*pow (2 + 2*l + k, -1)*
     pow (4 + 2*l + k, -1)*(384*(2*l + k)*(2 + 2*l + k)*(4 + 2*l + k) +
			    96*(2 + l)*(2 + 2*l + k)*(4 + 2*l + k)*pow (t, 2) +
			    12*(2 + l)*(4 + l)*(4 + 2*l + k)*
			    pow (t, 4) + (2 + l)*(4 + l)*(6 + l)*pow (t, 6))*
     pow (t, -1 + l))/384.,
    /* A*t + ((3*A - 4*(-1 + k)*pow(A,3))*pow(2 + k,-1)*pow(t,3))/12. + */
    /* (A*(15 - 40*(-1 + k)*pow(A,2) + 16*pow(A,4)*(1 - 3*k + 2*pow(k,2)))* */
    /*  pow(t,5)*pow(8 + 6*k + pow(k,2),-1))/160., */
    /* A + ((3*A - 4*(-1 + k)*pow(A,3))*pow(2 + k,-1)*pow(t,2))/4. + */
    /* (A*(15 - 40*(-1 + k)*pow(A,2) + 16*pow(A,4)*(1 - 3*k + 2*pow(k,2)))* */
    /*  pow(t,4)*pow(8 + 6*k + pow(k,2),-1))/32., */
    pow(t,l),
    l*pow(t,l-1.)
  };

  if (print){
    file = fopen(filename, "a");
    fprintf(file, "# A = %.15f\n# lambda = %.15f\n", A, bisec_param );
  }

  while (t < T_MAX)
    {
      int status =
	gsl_odeiv_evolve_apply (e, c, s,
				&sys,
				&t, T_MAX,
				&h, y);

      if (status != GSL_SUCCESS)
	break;
      /* are we still in the strip [0,pi]?  is the lienarized solution
	 reasonably boundaed?*/
      if ( 0. > y[0]
	   || y[0] > 3.15
	   || fabs(y[2]) > 10. )
	break;

      if (print /* && t_last+dt < t */)
	{
	  fprintf (file,
		   "%.15E %.15E %.15E %.15E %.15E\n",
		   t, y[0], y[1], y[2], y[3]);
	  t_last+=dt;
	  dt*=PRINT_DT_RATIO;
	}
      /* printf("%.15f\r",t); */
    }

  gsl_odeiv_evolve_free (e);
  gsl_odeiv_control_free (c);
  gsl_odeiv_step_free (s);
  if (print) {
    fprintf( file, "\n\n\n" );
    fclose( file );
  }

  return y[2];
}

int
func_expander (double t, const double y[], double f[],
	       void *params)
{
  f[0] = y[1];
  f[1] = sin(2.*y[0])*l*(l+k-2.)/2./t/t-((k-1.)/t+t/2.)*y[1];
  return GSL_SUCCESS;
}

double
fevol_expander (double bisec_param, int print, char * filename, void * p)
{
  const gsl_odeiv_step_type * T
    = STEPPER;

  FILE * file = NULL;

  gsl_odeiv_step * s
    = gsl_odeiv_step_alloc (T, 2);
  gsl_odeiv_control * c
    = gsl_odeiv_control_y_new (STEPPER_ERROR, 0.0);
  gsl_odeiv_evolve * e
    = gsl_odeiv_evolve_alloc (2);

  double dt=PRINT_DT, t_last=0.;

  gsl_odeiv_system sys = {func_expander, jac_dummy, 2, p};

  double t = T0;
  double h = H0;
  double A = bisec_param;
  double y[2] = {
    (A*(384 + l*pow(k + 2*l,-1)*pow(2 + k + 2*l,-1)*pow(4 + k + 2*l,-1)*
	pow(t,2)*(-96*(2 + k + 2*l)*(4 + k + 2*l) +
		  12*(2 + l)*(4 + k + 2*l)*pow(t,2) - (2 + l)*(4 + l)*pow(t,4)))*
     pow(t,l))/384.,
    (A*l*pow(k + 2*l,-1)*pow(2 + k + 2*l,-1)*
     pow(4 + k + 2*l,-1)*(384*(k + 2*l)*(2 + k + 2*l)*(4 + k + 2*l) -
			  96*(2 + l)*(2 + k + 2*l)*(4 + k + 2*l)*pow(t,2) +
			  12*(2 + l)*(4 + l)*(4 + k + 2*l)*pow(t,4) -
			  (2 + l)*(4 + l)*(6 + l)*pow(t,6))*pow(t,-1 + l))/384.
  };

  if (print){
    file = fopen(filename, "a");
    fprintf(file, "# A = %.15f\n", bisec_param );
  }

  while (t < T_MAX)
    {
      int status =
	gsl_odeiv_evolve_apply (e, c, s,
				&sys,
				&t, T_MAX,
				&h, y);

      if (status != GSL_SUCCESS)
	break;
      /* are we still in the strip [0,pi]? */
      if ( 0. > y[0] || y[0] > 3.15 )
	break;

      if (print /* && t_last+dt < t */)
	{
	  fprintf (file,
		   "%.15f %.15f %.15f\n",
		   t, y[0], y[1]/* , y[2], y[3] */);
	  t_last+=dt;
	  dt*=PRINT_DT_RATIO;
	}
      /* printf("%.15f\r",t); */
    }

  gsl_odeiv_evolve_free (e);
  gsl_odeiv_control_free (c);
  gsl_odeiv_step_free (s);

  if (print) {
    fprintf( file, "\n\n\n" );
    fclose( file );
  }

  return y[0];
}

int
func_static (double t, const double y[], double f[],
	       void *params)
{
  f[0] = y[1];
  f[1] = sin(2.*y[0])*l*(l+k-2.)/2./t/t-(k-1.)/t*y[1];
  return GSL_SUCCESS;
}

double
fevol_static (double L, int print, char * filename, void * p)
{
  const gsl_odeiv_step_type * T
    = STEPPER;

  FILE * file = NULL;

  gsl_odeiv_step * s
    = gsl_odeiv_step_alloc (T, 2);
  gsl_odeiv_control * c
    = gsl_odeiv_control_y_new (STEPPER_ERROR, 0.0);
  gsl_odeiv_evolve * e
    = gsl_odeiv_evolve_alloc (2);

  double dt=PRINT_DT, t_last=0.;

  gsl_odeiv_system sys = {func_static, jac_dummy, 2, p};

  double t = T0;
  double h = H0;
  double y[2] = {
    t,
    1.
  };

  if (print){
    file = fopen(filename, "a");
  }

  while (t < L)
    {
      int status =
	gsl_odeiv_evolve_apply (e, c, s,
				&sys,
				&t, L,
				&h, y);

      if (status != GSL_SUCCESS)
	break;
      /* are we still in the strip [0,pi]? */
      if ( 0. > y[0] || y[0] > 3.15 )
	break;

      if (print /* && t_last+dt < t */)
	{
	  fprintf (file,
		   "%.15f %.15f %.15f\n",
		   t, y[0], y[1]/* , y[2], y[3] */);
	  t_last+=dt;
	  dt*=PRINT_DT_RATIO;
	}
      printf("%.15f\r",t);
    }

  gsl_odeiv_evolve_free (e);
  gsl_odeiv_control_free (c);
  gsl_odeiv_step_free (s);

  if (print) {
    fprintf( file, "\n\n\n" );
    fclose( file );
  }

  return y[0];
}
