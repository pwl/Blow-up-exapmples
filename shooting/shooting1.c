#include "shooting1.h"


int
main (void)
{
  double results[10];
  double A;
  int i, results_collected;
  char shrinkerfile_name[1000];
  FILE * eigenfile;

  k=3.;
  l=1.;

  /* results[0]=12.103942232924556; */
  /* eigenfile = fopen("Expander_asymptotics.dat", "w"); */

  /* for( A=.001; A<1.e10; A*=1.01) */
  /*   { */
  /*     printf( "%.15f\n",A); */

  /*     fprintf( eigenfile, */
  /* 	       "%.15E %.15E\n", */
  /* 	       A, */
  /* 	       fevol_expander(A,0,NULL,(void*)(results))); */
  /* 	       /\* fevol_static_harmonic(A, 0, NULL, (void*)(results))); *\/ */
  /*   } */


  /* fclose(eigenfile); */

  for( l = 1.; l <=1.01; l+=1. )
    {
      for( k = 3.; k <= 3.01; k+=1. )
  	{
  	  printf(RED1 "searching for solutions to NODE in dimension %.3f with l=%.1f\n" FORMAT_OFF,k,l);
  	  /* results_collected = harvester */
  	  /*   ( 10., */
  	  /*     1.e-10, */
  	  /*     .9, */
  	  /*     RIPPER_EXP, */
  	  /*     10, */
  	  /*     results, */
  	  /*     0., */
  	  /*     fevol_static_harmonic, */
  	  /*     NULL ); */

	  results_collected = harvester
  	    ( .1,
  	      1.e10,
  	      3.,
  	      RIPPER_EXP,
  	      10,
  	      results,
  	      0.,
  	      fevol_static_harmonic,
  	      NULL );

  	  sprintf
  	    ( shrinkerfile_name,
  	      HARVESTER_DATA_DIR PROFILE_FILE_PREFIX HARVESTER_DEFAULT_EXTENSION,
  	      k, l);

  	  fclose(fopen( shrinkerfile_name, "w" ));

  	  for (i = 0; i < results_collected; ++i)
  	    {
  	      print_profile( results[i], fevol_static_harmonic );

  	      /* solve_eigenproblem(results[i], i+1, i+1, fevol_harmonic_eigenproblem); */
  	    }
  	}
    }


  return 0;
}
