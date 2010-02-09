#include "shooting1.h"

double k = 3.;

int
main (void)
{
  FILE * eigenfile;
  double results[10];
  /* double A; */
  double eigen_results[20];
  int i, j, results_collected, eigen_results_collected;
  char eigenfile_name[40];


  /* double b = bisec ( 100., 0.1, 6., 1.e-14, 0. ); */
  /* printf( "bisect=%.15f\n", b); */

  k=3.;

  l=2.;

  for( k = 3.; k <= 6.7; k+=.1 )
    {
      printf(RED1 "searching for solutions to NODE in dimension %.3f\n" FORMAT_OFF,k);
      results_collected = harvester
	( 1.e-10,
	  1.e10,
	  1.e-5,
	  RIPPER_DENSE1,
	  2,
	  results,
	  0.,
	  fevol_shrinker,
	  NULL );

      /* eigenfile = fopen("test.dat", "w"); */

      /* for( A=2.; A>0.; A-=.01) */
      /* 	{ */
      /* 	  fprintf( eigenfile, */
      /* 		   "%.15f %.15f\n", */
      /* 		   A,fevol_shrinker_eigenproblem(A, 0, NULL, (void*)(results))); */
      /* 	} */
      /* fclose(eigenfile); */

      for (i = 1; i < results_collected; ++i)
	{
	  print_shrinker_profile( results[i] );

	  printf(GREEN1 "solving eigenproblem for index %i in dimension %.3f\n" FORMAT_OFF,i+1,k);
	  eigen_results_collected = harvester
	    ( 200.,
	      -10.,
	      1.,
	      RIPPER_LINEAR,
	      10,
	      eigen_results,
	      0.,
	      fevol_shrinker_eigenproblem,
	      (void*)(results + i) );

	  sprintf
	    ( eigenfile_name,
	      HARVESTER_DATA_DIR "eigen" HARVESTER_DEFAULT_EIGEN_EXTENSION,
	      k, (int)l, i+1);
	  /* sprintf( eigenfile_name, HARVESTER_DATA_DIR "eigen_k"); */
	  eigenfile = fopen( eigenfile_name, "w" );

	  for ( j = 0; j < eigen_results_collected; j++ )
	    {
	      fprintf
		(eigenfile,
		 "%.15f %i %i %.15f %i %.15f\n",
		 k, (int)l, i+1, results[i], j+1, eigen_results[j] );
	    }

	  fclose( eigenfile );
	}
    }

  /* for( x=0.; x<100.; x+=1. ) */
  /*   { */
  /*     printf("%.15f\n",x); */
  /*     fprintf(f,"%.15f %.15f\n",x,fevol(100.,x,0)); */
  /*   } */

  /* fevol ( 100., b, 1 ); */
  /* fevol ( 100000., 1., 1 ); */
  /* fevol ( 100./.01, 2.7387531258849548976*.01, 1 ); */
  /* fevol ( 100., 2.7387531258849548976, 1 ); */
  return 0;
}
