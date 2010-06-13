#include "shooting1.h"


int
main (void)
{
  double results[100];
  double A;
  int i, results_collected;
  char shrinkerfile_name[1000];
  FILE * eigenfile;
  FILE * common_data;

  k=3.;
  l=1.;

  /* fevol_static(1.e15, 1, "harvester_data/static.dat", NULL); */

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

  common_data = fopen("harvester_data/common_data", "w");
  fprintf(common_data, "k   l   n   a\n");

  for( l = 1.; l <= 1.01; l+=1. )
    {
      for( k = 3.; k <= 3.01; k+=1. )
  	{
  	  printf(RED1 "searching for solutions to NODE in dimension %.3f with l=%.1f\n" FORMAT_OFF,k,l);
  	  results_collected = harvester
  	    ( .1,
  	      1.e100,
  	      3.,
  	      RIPPER_EXP,
  	      20,
  	      results,
  	      0.,
  	      fevol_shrinker,
  	      NULL );

  	  sprintf
  	    ( shrinkerfile_name,
  	      HARVESTER_DATA_DIR PROFILE_FILE_PREFIX HARVESTER_DEFAULT_EXTENSION,
  	      k, l);

  	  fclose(fopen( shrinkerfile_name, "w" ));

	  for (i = 0; i < results_collected; ++i)
  	    {
  	      print_profile( results[i], fevol_shrinker );
	      fprintf(common_data, "%.0f   %.0f   %i   %.15G\n", k, l, i, results[i] );

  	      /* solve_eigenproblem(results[i], i+1, i+2, fevol_shrinker_eigenproblem); */
  	    }
  	}
    }

  fclose(common_data);


  return 0;
}
