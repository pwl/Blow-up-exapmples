#include "shooting.h"

/* WARNING: All series expressions depending on l are valid for l=1
   only! */

void
solve_shrinker_eigenproblem
(double A,
 int index,
 int eigenval_number)
{
  FILE *eigenfile;
  int j, eigen_results_collected;
  double eigen_results[100];
  char eigenfile_name[100];

  printf(GREEN1 "solving eigenproblem for index %i in dimension %.3f with l=%.1f\nLambda > 1\n" FORMAT_OFF,
	 index, k, l);

  eigen_results_collected = harvester
    ( 2.,
      1.e10,
      2.,
      RIPPER_EXP,
      index-1,
      eigen_results,
      0.,
      fevol_shrinker_eigenproblem,
      (void*)(&A) );

  printf(GREEN1 "Lambda <= 1\n" FORMAT_OFF);

  eigen_results_collected += harvester
    ( 10.,
      -1.e10,
      .2,
      RIPPER_LINEAR,
      eigenval_number-eigen_results_collected,
      eigen_results+eigen_results_collected,
      0.,
      fevol_shrinker_eigenproblem,
      (void*)(&A) );

  sprintf
    ( eigenfile_name,
      HARVESTER_DATA_DIR "eigen" HARVESTER_DEFAULT_EIGEN_EXTENSION,
      k, l, index);

  eigenfile = fopen( eigenfile_name, "a" );

  for ( j = index-2; j >= 0; j-- )
    {
    fprintf
	(eigenfile,
	 "%.15f %.1f %i %.15f %i %.15f\n",
	 k, l, index, A, index-j-1, eigen_results[j] );
    }

  for ( j = index-1; j < eigen_results_collected; j++ )
    {
      fprintf
	(eigenfile,
	 "%.15f %.1f %i %.15f %i %.15f\n",
	 k, l, index, A, j+1, eigen_results[j] );
    }

  fclose( eigenfile );
}

void
solve_eigenproblem
( double A,
  int index,
  int eigenval_number,
  double (*fevol_eigenproblem)(double, int, char *, void *) )
{
  FILE *eigenfile;
  int j, eigen_results_collected;
  double eigen_results[100];
  char eigenfile_name[500];

  printf(GREEN1 "solving eigenproblem for index %i in dimension %.3f with l=%.1f\nLambda > 1\n" FORMAT_OFF,
	 index, k, l);

  eigen_results_collected = harvester
    ( 2.,
      1.e10,
      50.,
      RIPPER_LINEAR,
      index-1,
      eigen_results,
      0.,
      fevol_eigenproblem,
      (void*)(&A) );

  /* eigen_results_collected = harvester */
  /*   ( 2., */
  /*     1.e100, */
  /*     1.1, */
  /*     RIPPER_EXP, */
  /*     index-1, */
  /*     eigen_results, */
  /*     0., */
  /*     fevol_eigenproblem, */
  /*     (void*)(&A) ); */

  printf(GREEN1 "Lambda <= 1\n" FORMAT_OFF);

  eigen_results_collected += harvester
    ( 2.,
      -1.e10,
      .1,
      RIPPER_LINEAR,
      eigenval_number-eigen_results_collected,
      eigen_results+eigen_results_collected,
      0.,
      fevol_eigenproblem,
      (void*)(&A) );

  sprintf
    ( eigenfile_name,
      HARVESTER_DATA_DIR "eigen" HARVESTER_DEFAULT_EIGEN_EXTENSION,
      k, l, index);

  eigenfile = fopen( eigenfile_name, "w" );
  fclose( eigenfile );

  for ( j = index-2; j >= 0; j-- )
    {
      eigenfile = fopen( eigenfile_name, "a" );
      fprintf
  	(eigenfile,
  	 "#%.15f %.1f %i %.15f %i %.15f\n",
  	 k, l, index, A, index-j-1, eigen_results[j] );
      fclose( eigenfile );

      fevol_eigenproblem(eigen_results[j], 1, eigenfile_name, (void*)(&A));
    }

  for ( j = index-1; j < eigen_results_collected; j++ )
    {
      eigenfile = fopen( eigenfile_name, "a" );
      fprintf
  	(eigenfile,
  	 "#%.15f %.1f %i %.15f %i %.15f\n",
  	 k, l, index, A, j+1, eigen_results[j] );
      fclose( eigenfile );

      fevol_eigenproblem(eigen_results[j], 1, eigenfile_name, (void*)(&A));
    }

}


void print_shrinker_profile( double A ) {
  char shrinkerfile_name[500];
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

void
print_profile( double A,
	       double (*fevol)(double, int, char *, void *))
{
  char profile_file_name[500];
  FILE *profile_file;

  sprintf
    ( profile_file_name,
      HARVESTER_DATA_DIR PROFILE_FILE_PREFIX HARVESTER_DEFAULT_EXTENSION,
      k, l);
  profile_file = fopen( profile_file_name, "a" );
  fprintf( profile_file, "# k = %.15f\n# l = %.1f\n", k, l );
  fclose( profile_file );
  fevol( A, 1 , profile_file_name, NULL );
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
      printf( "%03i, A=%.20G, delta/A=%.1G\r",
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


  if( results_collected >= results_max )
    return results_collected;

  value_last = fevol( cursor, 0, "", param ) - val;

  switch( opt )
    {
    case RIPPER_LINEAR:
      dt = delta/fabs(b-a);
      break;
    case RIPPER_DENSE1:
      dt = sqrt(1.-pow(1.-delta/fabs(b-a),2));
      break;
    case RIPPER_EXP:
      dt = log(delta)/log(b/a);
      break;
    default:
      dt = log(delta);
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
      case RIPPER_EXP:
      	s=a*(pow(b/a,t)-1)/(b-a);
      	break;
      default:
	s=t;
      }

      cursor_last = cursor;
      cursor = a+s*(b-a);
      value = fevol( cursor, 0, "", param ) - val;

      printf(/* "                              \r" */ "cursor = %.10G\r", cursor);

      /* printf("cursor_last = %.5E, cursor = %.5E, dt = %.5E, s = %.5E\n", cursor_last, cursor, dt, s); */
      if( results_collected >= results_max )
	return results_collected;

      /* if signs are opposite */
      if( value_last*value < 0 )
      	{
	  results[results_collected] =
	    bisec( cursor_last, cursor, epsilon, val, fevol, param );

	  results_collected++;
	  printf(CYAN1 "collected result " BOLD1 "%.15G" FORMAT_OFF CYAN1 ", %i results in total\n" FORMAT_OFF,
		 results[results_collected-1], results_collected);

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
  f[1] = (sin(2.*y[0])*l*(l+k-2.)/2./t-(k-1.)*y[1])/t+t/2.*y[1];
  /* f[1] = sin(2.*y[0])*l*(l+k-2.)/2./t/t-((k-1.)/t-t/2.)*y[1]; */
  return GSL_SUCCESS;
}

double
fevol_shrinker (double A, int print, char * filename, void * p)
{
  const gsl_odeiv_step_type * T
    = STEPPER;

  FILE * file = NULL;

  gsl_odeiv_step * s
    = gsl_odeiv_step_alloc (T, 2);
  gsl_odeiv_control * c
    = gsl_odeiv_control_y_new (STEPPER_ERROR, STEPPER_ERROR);
  gsl_odeiv_evolve * e
    = gsl_odeiv_evolve_alloc (2);

  double dt=.01/* T0/A *//* PRINT_DT */, t_last=0.;

  gsl_odeiv_system sys = {func_shrinker, jac_dummy, 2, p};

  double En= 0.;
  double t = T0/A;
  double h = H0/A;
  double y[2] = {
    A*t + ((3*A - 4*(-1 + k)*pow(A,3))*pow(2 + k,-1)*pow(t,3))/
     12. + (A*(15 + 8*(-1 + k)*pow(A,2)*
          (-5 + (-2 + 4*k)*pow(A,2)))*pow(2 + k,-1)*pow(4 + k,-1)*
       pow(t,5))/160.,A + ((3*A - 4*(-1 + k)*pow(A,3))*
       pow(2 + k,-1)*pow(t,2))/4. +
    (A*(15 + 8*(-1 + k)*pow(A,2)*(-5 + (-2 + 4*k)*pow(A,2)))*
       pow(2 + k,-1)*pow(4 + k,-1)*pow(t,4))/32.
    /* (A + A*l*pow (8*l + 4*k, -1)* */
    /*  pow (t, 2) + (A*l*(2 + l)*pow (2*l + k, -1)*pow (2 + 2*l + k, -1)* */
    /* 		   pow (t, 4))/ */
    /*  32. + (A*l*(2 + l)*(4 + l)*pow (2*l + k, -1)* */
    /* 	    pow (2 + 2*l + k, -1)*pow (4 + 2*l + k, -1)*pow (t, 6))/ */
    /*  384.)*pow (t, l), */

    /* (A*l*pow (2*l + k, -1)*pow (2 + 2*l + k, -1)* */
    /*  pow (4 + 2*l + k, -1)*(384*(2*l + k)*(2 + 2*l + k)*(4 + 2*l + k) + */
    /* 			    96*(2 + l)*(2 + 2*l + k)*(4 + 2*l + k)*pow (t, 2) + */
    /* 			    12*(2 + l)*(4 + l)*(4 + 2*l + k)* */
    /* 			    pow (t, 4) + (2 + l)*(4 + l)*(6 + l)*pow (t, 6))* */
    /*  pow (t, -1 + l))/384. */
  };


  if (print){
    file = fopen(filename, "a");
    fprintf(file, "# A = %.15G\n", A );
    fprintf (file,"0. 0. 0.\n");
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
      if ( 0. > y[0] || y[0] > PI )
	break;

      if (print)
	{
	  En+=h*(y[1]*y[1]/2.+(k-1.)*sin(y[0])*sin(y[0])/t/t)*exp(-t*t/4.)*pow(t,k-1)/2.;
	  if (t > t_last+dt )
	    {
	      fprintf (file,
		       "%.15G %.15G %.15G\n",
		       t, y[0], y[1]/* , y[2], y[3] */);
	      t_last+=dt;
	      /* dt*=PRINT_DT_RATIO; */
	    }
	}
      /* printf("%.15f\r",t); */
    }

  gsl_odeiv_evolve_free (e);
  gsl_odeiv_control_free (c);
  gsl_odeiv_step_free (s);

  if (print) {
    fprintf( file, "# En = %.15G\n\n\n\n", En );
    fclose( file );
  }

  return y[1]*t*t;
}

int
func_shrinker_reverse (double t, const double y[], double f[],
	       void *params)
{
  f[0] = y[1];
  f[1] = (k-1)/2.*sin(2*y[0])+(k-2.-.5*exp(-2*t))*f[0];
  /* f[1] = sin(2.*y[0])*l*(l+k-2.)/2./t/t-((k-1.)/t-t/2.)*y[1]; */
  return GSL_SUCCESS;
}

double
fevol_shrinker_reverse (double bisec_param, int print, char * filename, void * p)
{
  const gsl_odeiv_step_type * T
    = STEPPER;

  FILE * file = NULL;

  gsl_odeiv_step * s
    = gsl_odeiv_step_alloc (T, 2);
  gsl_odeiv_control * c
    = gsl_odeiv_control_y_new (STEPPER_ERROR, STEPPER_ERROR);
  gsl_odeiv_evolve * e
    = gsl_odeiv_evolve_alloc (2);


  gsl_odeiv_system sys = {func_shrinker_reverse, jac_dummy, 2, p};

  /* double En= 0.; */
  double t = T0;
  double h = H0;
  double A = bisec_param;
  double y[2] = {
    PI/2.+A-(k-1)/2.*sin(2*A)*exp(2.*t),
    (k-1)*sin(2*A)*exp(2.*t)
  };
  double dt=.01/* PRINT_DT */, t_last=t;

  if (print){
    file = fopen(filename, "a");
    fprintf(file, "# A = %.15G\n", bisec_param );
  }

  while ( t < T_MAX )
    {
      int status =
	gsl_odeiv_evolve_apply (e, c, s,
				&sys,
				&t, T_MAX,
				&h, y);

      if (status != GSL_SUCCESS)
	break;
      /* are we still in the strip [0,pi]? */
      if ( y[0] > PI || y[0] < 0. )
      	break;

      if (print)
	{
	  /* En+=h*(y[1]*y[1]/2.+(k-1.)*sin(y[0])*sin(y[0])/t/t)*exp(-t*t/4.)*pow(t,k-1)/2.; */
	  if (t > t_last )
	    {
	      fprintf (file,
		       "%.15G %.15G %.15G\n",
		       -t, y[0], y[1]/* , y[2], y[3] */);
	      t_last+=dt;
	      /* dt*=PRINT_DT_RATIO; */
	    }
	}
      /* printf("%.15f\r",t); */
    }

  gsl_odeiv_evolve_free (e);
  gsl_odeiv_control_free (c);
  gsl_odeiv_step_free (s);

  if (print) {
    fprintf( file, "\n\n\n");
    fclose( file );
  }

  return y[0];
}

int
func_shrinker_eigenproblem_reverse
(double t,
 const double y[],
 double f[],
 void *params)
{
  double lambda = *(double*)params;
  f[0] = y[1];
  f[1] = (k-1)/2.*sin(2*y[0])+(k-2.-.5*exp(-2*t))*y[1];
  f[2] = y[3];
  f[3] = ((k-1)*cos(2*y[0])+lambda*exp(-2.*t))*y[2]+(k-2.-.5*exp(-2*t))*y[3];
  return GSL_SUCCESS;
}

double
fevol_shrinker_eigenproblem_reverse
(double lbd,
 int print,
 char * filename,
 void * p)
{
  const gsl_odeiv_step_type * T
    = STEPPER;

  FILE * file = NULL;

  gsl_odeiv_step * s
    = gsl_odeiv_step_alloc (T, 4);
  gsl_odeiv_control * c
    = gsl_odeiv_control_y_new (STEPPER_ERROR, STEPPER_ERROR);
  gsl_odeiv_evolve * e
    = gsl_odeiv_evolve_alloc (4);


  gsl_odeiv_system sys = {func_shrinker_eigenproblem_reverse, jac_dummy, 4, (void*)&lbd};

  /* double En= 0.; */
  double t = T0;
  double h = H0;
  double A = *(double*)p;
  double y[4] = {
    PI/2.+A-(k-1)/2.*sin(2*A)*exp(2.*t),
    (k-1)*sin(2*A)*exp(2.*t),
    exp(-2.*lbd*t),
    -2.*lbd*exp(-2.*lbd*t)
  };
  double dt=.01/* PRINT_DT */, t_last=t;

  if ( print )
    {
      file = fopen(filename, "a");
      fprintf(file, "# A = %.15G\n", lbd );
    }

  while ( t < -5. )
    {
      int status =
	gsl_odeiv_evolve_apply (e, c, s,
				&sys,
				&t, T_MAX,
				&h, y);

      if (status != GSL_SUCCESS)
	break;

      /* are we still in the strip [0,pi]? */
      if ( y[0] > PI || y[0] < 0. || fabs(y[2]) > 1.e10)
	{
	  /* printf("bubel at %.15G\n", t); */
	  break;
	}


      if (print)
	{
	  if ( t > t_last )
	    {
	      fprintf (file,
		       "%.15G %.15G %.15G %.15G %.15G\n",
		       -t, y[0], y[1], y[2], y[3]);
	      t_last+=dt;
	      /* dt*=PRINT_DT_RATIO; */
	    }
	}
      /* printf("%.15f\r",t); */
    }

  gsl_odeiv_evolve_free (e);
  gsl_odeiv_control_free (c);
  gsl_odeiv_step_free (s);

  if (print) {
    fprintf( file, "\n\n\n");
    fclose( file );
  }

  return y[2]/* *y[2]*exp(-(k-1.)*t)*exp(-exp(2.*t)/4.) */;
}

double
expander_asymptotics( double A, int printf, char * filename, void * p )
{
  return d1_f(expander_asymptotics_wrapper,A,A*H0)*pow(A,k/2.);
}

double
expander_asymptotics_wrapper(double A)
{
  return fevol_expander(A,0,NULL,NULL);
}

double
d1_f( double(*f)(double x), double x, double h )
{
  return (f(x+h)-f(x-h))/2./h;
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
fevol_shrinker_eigenproblem (double L, int print, char * filename, void * p)
{
  const gsl_odeiv_step_type * T
    = STEPPER;

  FILE * file = NULL;

  gsl_odeiv_step * s
    = gsl_odeiv_step_alloc (T, 4);
  gsl_odeiv_control * c
    = gsl_odeiv_control_y_new (STEPPER_ERROR, STEPPER_ERROR);
  gsl_odeiv_evolve * e
    = gsl_odeiv_evolve_alloc (4);

  double dt=PRINT_DT, t_last=0.;

  gsl_odeiv_system sys = {func_shrinker_eigenproblem, jac_dummy, 4, (void*)&L};

  double A = *(double*)p;
  double t = T0/A;
  double h = H0/A;
  double y[4] = {
    A*t + ((3*A - 4*(-1 + k)*pow(A,3))*pow(2 + k,-1)*pow(t,3))/
     12. + (A*(15 + 8*(-1 + k)*pow(A,2)*
          (-5 + (-2 + 4*k)*pow(A,2)))*pow(2 + k,-1)*pow(4 + k,-1)*
       pow(t,5))/160.,A + ((3*A - 4*(-1 + k)*pow(A,3))*
       pow(2 + k,-1)*pow(t,2))/4. +
    (A*(15 + 8*(-1 + k)*pow(A,2)*(-5 + (-2 + 4*k)*pow(A,2)))*
       pow(2 + k,-1)*pow(4 + k,-1)*pow(t,4))/32.,
   t + ((1 + 2*L - 4*(-1 + k)*pow(A,2))*pow(2 + k,-1)*pow(t,3))/
     4. + ((3 + 4*L*(2 + L) - 8*(-1 + k)*(3 + 2*L)*pow(A,2) +
         16*(-1 + k)*(-1 + 2*k)*pow(A,4))*pow(2 + k,-1)*
       pow(4 + k,-1)*pow(t,5))/32.,
   1 + ((3 + 6*L - 12*(-1 + k)*pow(A,2))*pow(2 + k,-1)*pow(t,2))/
     4. + (5*(3 + 4*L*(2 + L) - 8*(-1 + k)*(3 + 2*L)*pow(A,2) +
         16*(-1 + k)*(-1 + 2*k)*pow(A,4))*pow(2 + k,-1)*
       pow(4 + k,-1)*pow(t,4))/32.
  };

  if (print){
    file = fopen(filename, "a");
    fprintf(file, "# A = %.15f\n# lambda = %.15f\n", A, L );
  }

  while (t < 12.)
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
	   || y[0] > PI
	   /* || ( t > 5. && fabs(y[2])*pow(t,2*L) > 1.) */)
	break;

      if (print /* && t_last+dt < t */)
	{
	  fprintf (file,
		   "%.15E %.15E %.15E %.15E %.15E\n",
		   t, y[0], y[1], y[2]/* *pow(t,2*L) */, y[3]);
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

  return /* y[2] */(y[3]/y[2]*t+2.*L)*y[2]/* +2.*L/t *//* *pow(t,2*L)*/;
}

int
func_expander_eigenproblem (double t, const double y[], double f[],
			    void *params)
{
  double lambda = *(double*)params;
  f[0] = y[1];
  f[1] = l*(l+k-2.)/2.*sin(2.*y[0])/t/t-((k-1.)/t+t/2.)*y[1];
  f[2] = y[3];
  f[3] = (l*(l+k-2.)*cos(2.*y[0])/t/t+lambda)*y[2]-((k-1.)/t+t/2.)*y[3];
  /* below we assume l=1 */
  /* f[3] = ((pow(2*k+t*t,2)-4-32.*(k-1.)*pow(sin(y[0]),2))/t/t/16.+lambda)*y[2]; */
  return GSL_SUCCESS;
}

double
fevol_expander_eigenproblem (double bisec_param, int print, char * filename, void * p)
{
  const gsl_odeiv_step_type * T
    = STEPPER;

  FILE * file = NULL;

  gsl_odeiv_step * s
    = gsl_odeiv_step_alloc (T, 4);
  gsl_odeiv_control * c
    = gsl_odeiv_control_y_new (STEPPER_ERROR, STEPPER_ERROR);
  gsl_odeiv_evolve * e
    = gsl_odeiv_evolve_alloc (4);

  double dt=PRINT_DT, t_last=0.;

  gsl_odeiv_system sys = {func_expander_eigenproblem, jac_dummy, 4, (void*)&bisec_param};

  double A = *(double*)p;
  double L = bisec_param;
  double t = T0/A;
  double h = H0/A;
  double y[4] = {		      /* expressions derived using
					 ~/SeriesSolve.nb */
    A*t - ((3*A + 4*(-1 + k)*pow(A,3))*pow(2 + k,-1)*pow(t,3))/
     12. + (A*(15 + 8*(-1 + k)*pow(A,2)*
          (5 + (-2 + 4*k)*pow(A,2)))*pow(2 + k,-1)*pow(4 + k,-1)*
       pow(t,5))/160.,A - ((3*A + 4*(-1 + k)*pow(A,3))*
       pow(2 + k,-1)*pow(t,2))/4. +
    (A*(15 + 8*(-1 + k)*pow(A,2)*(5 + (-2 + 4*k)*pow(A,2)))*
       pow(2 + k,-1)*pow(4 + k,-1)*pow(t,4))/32.,
   t + ((-1 + 2*L - 4*(-1 + k)*pow(A,2))*pow(2 + k,-1)*pow(t,3))/
     4. + ((3 + 4*(-2 + L)*L - 8*(-1 + k)*(-3 + 2*L)*pow(A,2) +
         16*(-1 + k)*(-1 + 2*k)*pow(A,4))*pow(2 + k,-1)*
       pow(4 + k,-1)*pow(t,5))/32.,
   1 + ((-3 + 6*L - 12*(-1 + k)*pow(A,2))*pow(2 + k,-1)*pow(t,2))/
     4. + (5*(3 + 4*(-2 + L)*L - 8*(-1 + k)*(-3 + 2*L)*pow(A,2) +
         16*(-1 + k)*(-1 + 2*k)*pow(A,4))*pow(2 + k,-1)*
       pow(4 + k,-1)*pow(t,4))/32.
  };

  if (print){
    file = fopen(filename, "a");
    fprintf(file, "# A = %.15f\n# lambda = %.15f\n", A, bisec_param );
  }

  while (t < 50.)
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
	   || y[0] > PI
	   || (t > 20. && fabs(y[2])/ /* pow(t,-2*bisec_param-(k+1)/2)/ */exp(-t*t/8.)/* *pow(t,(k-1.)/2.)*exp(t*t/8.) */ > 1.e5))
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

  return y[2]/exp(-t*t/8.)
    /* *pow(t,(k-1.)/2.)*exp(t*t/8.) */;
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
    = gsl_odeiv_control_y_new (STEPPER_ERROR, STEPPER_ERROR);
  gsl_odeiv_evolve * e
    = gsl_odeiv_evolve_alloc (2);

  double dt=PRINT_DT, t_last=0.;

  gsl_odeiv_system sys = {func_expander, jac_dummy, 2, (void*)&bisec_param};

  double A = bisec_param;
  double t = T0/A;
  double h = H0/A;
  double y[2] = {
    A*t - ((3*A + 4*(-1 + k)*pow(A,3))*pow(2 + k,-1)*pow(t,3))/
     12. + (A*(15 + 8*(-1 + k)*pow(A,2)*
          (5 + (-2 + 4*k)*pow(A,2)))*pow(2 + k,-1)*pow(4 + k,-1)*
       pow(t,5))/160.,A - ((3*A + 4*(-1 + k)*pow(A,3))*
       pow(2 + k,-1)*pow(t,2))/4. +
    (A*(15 + 8*(-1 + k)*pow(A,2)*(5 + (-2 + 4*k)*pow(A,2)))*
       pow(2 + k,-1)*pow(4 + k,-1)*pow(t,4))/32.
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
      if ( fabs(y[0]-PI/2.) > PI/2. )
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

int
func_static_harmonic (double t, const double y[], double f[],
		      void *params)
{
  f[0] = y[1];
  f[1] = sin(2.*y[0])*l*(l+k-2.)/2./sin(t)/sin(t)-(k-1.)/tan(t)*y[1];
  return GSL_SUCCESS;
}

int
func_static_harmonic_eigenproblem
(double t,
 const double y[],
 double f[],
 void *params)
{
  double lambda = *(double*)params;
  f[0] = y[1];
  f[1] = sin(2.*y[0])*l*(l+k-2.)/2./sin(t)/sin(t)-(k-1.)/tan(t)*y[1];
  f[2] = y[3];
  f[3] = (l*(l+k-2.)*cos(2.*y[0])/sin(t)/sin(t)+lambda)*y[2]-(k-1.)/tan(t)*y[3];
  return GSL_SUCCESS;
}

double
fevol_static_harmonic (double A, int print, char * filename, void * p)
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

  gsl_odeiv_system sys = {func_static_harmonic, jac_dummy, 2, p};

  double t = T0;
  double h = H0;
  double y[2] = {
    A*t*(1.-(k-1)*(A*A-1)/3./(k+2.)*t*t),
    A*(1.-2.*(k-1)*(A*A-1)/3./(k+2.)*t)
  };
  /* double y[2] = { */
  /*   PI/2., */
  /*   A */
  /* }; */

  if (print){
    file = fopen(filename, "a");
  }

  while (t < PI-.05*1.e-4)
    {
      int status =
	gsl_odeiv_evolve_apply (e, c, s,
				&sys,
				&t, PI,
				&h, y);

      if (status != GSL_SUCCESS)
	break;
      /* are we still in the strip [0,pi]? */
      if ( y[0] < 0.)
	{
	  y[0]=-1.;
	  break;
	}
      if ( y[0] > PI )
	{
	  y[0]=1.;
	  break;
	}


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

  return /* (y[0]-PI/2.)*y[1] */ y[0];
}

double
fevol_harmonic_eigenproblem (double bisec_param, int print, char * filename, void * p)
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

  gsl_odeiv_system sys = {func_static_harmonic_eigenproblem, jac_dummy, 4, (void*)&bisec_param};

  double t = T0;
  double h = H0;
  double A = *(double*)p;
  double y[4] = {
    A*t*(1.-(k-1)*(A*A-1)/3./(k+2.)*t*t),
    A*(1.-2.*(k-1)*(A*A-1)/3./(k+2.)*t),
    t,
    1.
  };
  /* double y[4] = { */
  /*   A, */
  /*   0., */
  /*   1., */
  /*   0. */

  /* }; */

  if (print){
    file = fopen(filename, "a");
    fprintf(file, "# A = %.15f\n# lambda = %.15f\n", A, bisec_param );
  }

  while (t < PI-1.e-3)
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
	   || y[0] > PI
	   || fabs(y[2]) > 1.e5)
	break;

      if (print /* && t_last+dt < t */)
	{
	  fprintf (file,
		   "%.15E %.15E %.15E %.15E %.15E\n",
		   t, y[0], y[1], y[2], y[3]);
	  t_last+=dt;
	  dt*=PRINT_DT_RATIO;
	}
      /* printf("%.15f %.15f\r",t, y[2]); */
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
func_shrinker_regularized (double t, const double y[], double f[],
	       void *params)
{
  f[0] = y[1];
  /* f[1] = (sin(2.*y[0])*l*(l+k-2.)/2./t-(k-1.)*y[1])/t+t/2.*y[1]; */
  f[1] = exp(-t*t/4.)*t*sin(2.*y[0]*exp(t*t/4.)/t/t/t)-(-4./t+t/2.)*y[1]-6.*y[0]/t/t;
  return GSL_SUCCESS;
}

double
fevol_shrinker_regularized (double A, int print, char * filename, void * p)
{
  const gsl_odeiv_step_type * T
    = STEPPER;

  FILE * file = NULL;

  gsl_odeiv_step * s
    = gsl_odeiv_step_alloc (T, 2);
  gsl_odeiv_control * c
    = gsl_odeiv_control_y_new (STEPPER_ERROR, STEPPER_ERROR);
  gsl_odeiv_evolve * e
    = gsl_odeiv_evolve_alloc (2);

  double dt=T0/A/* PRINT_DT */, t_last=0.;

  gsl_odeiv_system sys = {func_shrinker_regularized, jac_dummy, 2, p};

  double En= 0.;
  double t = T0/A;
  double h = H0/A;
  double y[2] = {
    pow(t,3)*exp(t*t/4.)*(A*t + ((3*A - 4*(-1 + k)*pow(A,3))*pow(2 + k,-1)*pow(t,3))/
     12. + (A*(15 + 8*(-1 + k)*pow(A,2)*
          (-5 + (-2 + 4*k)*pow(A,2)))*pow(2 + k,-1)*pow(4 + k,-1)*
	    pow(t,5))/160.),
    0.				/* filled in below */
  };

  double y_prev=y[0];

  y[1]=(3./t+t/2.)*y[0]+pow(t,3)*exp(t*t/4.)*(A + ((3*A - 4*(-1 + k)*pow(A,3))*
       pow(2 + k,-1)*pow(t,2))/4. +
    (A*(15 + 8*(-1 + k)*pow(A,2)*(-5 + (-2 + 4*k)*pow(A,2)))*
     pow(2 + k,-1)*pow(4 + k,-1)*pow(t,4))/32.);



  if (print){
    file = fopen(filename, "a");
    fprintf(file, "# A = %.15G\n", A );
  }

  while (t < T_MAX)
    {
      int status =
	gsl_odeiv_evolve_apply (e, c, s,
				&sys,
				&t, T_MAX,
				&h, y);

      if (status != GSL_SUCCESS || isnan(y[0]*y[1]))
	{
	  y[0]=y_prev;
	  break;
	}

      y_prev=y[0];

      /* are we still in the strip [0,pi]? */
      /* if ( 0. > y[0] || y[0] > PI ) */
      /* 	break; */

      if (print)
	{
	  /* En+=h*(y[1]*y[1]/2.+(k-1.)*sin(y[0])*sin(y[0])/t/t)*exp(-t*t/4.)*pow(t,k-1)/2.; */
	  if (t > t_last )
	    {
	      fprintf (file,
		       "%.15G %.15G %.15G\n",
		       t, y[0], y[1]/* , y[2], y[3] */);
	      t_last+=dt;
	      dt*=PRINT_DT_RATIO;
	    }
	}
      /* printf("%.15G %.15G\n",t,y[0]); */
    }

  gsl_odeiv_evolve_free (e);
  gsl_odeiv_control_free (c);
  gsl_odeiv_step_free (s);

  if (print) {
    /* fprintf( file, "# En = %.15G\n\n\n\n", En ); */
    fclose( file );
  }

  return y[0];
}
