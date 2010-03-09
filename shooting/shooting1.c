#include "shooting1.h"


int
main (void)
{
  double results[10];
  double A;
  int i, results_collected;
  char shrinkerfile_name[100];
  FILE * eigenfile;

  k=5.;
  l=2.;

  eigenfile = fopen("test.dat", "w");

  /* fevol_expander( 1., 1, "test.dat", NULL ); */

  /* for( A=1.e-5; A<20000.; A*=1.01) */
  /*   { */
  /*     printf( "%.15f\n",A); */

  /*     fprintf( eigenfile, */
  /* 	       "%.15f %.15f\n", */
  /* 	       A,fevol_expander(A, 0, NULL, (void*)(results))); */
  /*   } */

  fclose(eigenfile);

  for( l = 1.; l <=1.01; l+=1. )
    {
      for( k = 3.; k <= 6.01; k+=1. )
	{
	  printf(RED1 "searching for solutions to NODE in dimension %.3f with l=%.1f\n" FORMAT_OFF,k,l);
	  results_collected = harvester
	    ( 1.e-2,
	      1.e100,
	      5.,
	      RIPPER_EXP,
	      6,
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

	      /* solve_shrinker_eigenproblem(results[i], i+1, 6); */
	    }
	}
    }


  return 0;
}
