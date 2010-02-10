#include "shooting1.h"



int
main (void)
{
  double results[10];
  int i, results_collected;
  char shrinkerfile_name[100];

  l=2.5;

  /* eigenfile = fopen("test.dat", "w"); */

  /* for( A=0.; A<20.; A+=.01) */
  /*   { */
  /*     printf( "%.15f\r",A); */


  /*     fprintf( eigenfile, */
  /* 	       "%.15f %.15f\n", */
  /* 	       A,fevol_shrinker(A, 0, NULL, (void*)(results))); */
  /*   } */

  /* fclose(eigenfile); */

  for( l = 1.; l <=3.01; l+=.1 )
    {
      for( k = 3.; k <= 6.01; k+=.1 )
	{
	  printf(RED1 "searching for solutions to NODE in dimension %.3f with l=%.1f\n" FORMAT_OFF,k,l);
	  results_collected = harvester
	    ( 1.e-2,
	      1.e10,
	      1.e-2,
	      RIPPER_DENSE1,
	      3,
	      results,
	      0.,
	      fevol_shrinker,
	      NULL );

	  sprintf
	    ( shrinkerfile_name,
	      HARVESTER_DATA_DIR "shrinker" HARVESTER_DEFAULT_EXTENSION,
	      k, l);

	  fclose(fopen( shrinkerfile_name, "w" ));

	  for (i = 0; i < results_collected; ++i)
	    {
	      print_shrinker_profile( results[i] );

	      /* solve_shrinker_eigenproblem(results[i], i+1); */
	    }
	}
    }


  return 0;
}
