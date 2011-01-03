#include "shooting1.h"


int
main (void)
{
  double results[1000];
  double A;
  int i, results_collected;
  char shrinkerfile_name[1000];
  char common_file_name[1000];
  FILE * eigenfile;
  FILE * common_data;

  k=3.;
  l=1.;

  /* fclose(fopen("harvester_data/reverse.dat", "w")); */
  /* fevol_shrinker_reverse(.1, 1, "harvester_data/reverse.dat", NULL); */

  /* results[0]=12.103942232924556; */


  for( l = 1.; l <= 1.01; l+=1. )
    {
      for( k = 3.; k <= 3.; k+=1. )
  	{
	  /* sprintf(common_file_name,"Expander_asymptotics_k%.2f.dat", k); */
	  /* sprintf(common_file_name,"test.dat"); */
	  /* eigenfile = fopen(common_file_name, "w"); */
	  /* printf( "k=%.2f\n",k); */

	  /* for( A=.001; A<1.e10; A*=1.001) */
	  /*   { */

	  /*     printf( "%.15G %.15G\n", */
	  /* 	      A, */
	  /* 	      fevol_expander(A,0,NULL,NULL)); */
	  /*     fprintf( eigenfile, */
	  /* 	       "%.15G %.15G\n", */
	  /* 	       A, */
	  /* 	       fevol_expander(A,0,NULL,NULL)); */
	  /*     /\* fevol_static_harmonic(A, 0, NULL, (void*)(results))); *\/ */
	  /*   } */


	  /* fclose(eigenfile); */
	  /* sprintf(common_file_name, "harvester_data/expander_k%.2f.dat", k); */
	  /* common_data = fopen(common_file_name, "w"); */
	  /* fprintf(common_data, "k   l   n   a\n"); */
	  printf(RED1 "searching for solutions to NODE in dimension %.3f with l=%.1f\n" FORMAT_OFF,k,l);

  	  /* results_collected = harvester */
  	  /*   ( PI/2.-1.e-10, */
  	  /*     1.e-100, */
  	  /*     1./3., */
  	  /*     RIPPER_EXP, */
  	  /*     6, */
  	  /*     results, */
  	  /*     0., */
  	  /*     fevol_shrinker_reverse, */
  	  /*     NULL ); */

	  results_collected = harvester
  	    ( 1.e-10,
  	      1.e100,
  	      3.,
  	      RIPPER_EXP,
  	      4,
  	      results,
  	      0.,
  	      fevol_shrinker,
  	      NULL );

	  /* results_collected = harvester */
  	  /*   ( 1., */
  	  /*     1.e10, */
  	  /*     3., */
  	  /*     RIPPER_EXP, */
  	  /*     7, */
  	  /*     results, */
  	  /*     -0.57314113304, */
  	  /*     fevol_shrinker, */
  	  /*     NULL ); */

  	  sprintf
  	    ( shrinkerfile_name,
  	      HARVESTER_DATA_DIR PROFILE_FILE_PREFIX HARVESTER_DEFAULT_EXTENSION,
  	      k, l);

	  /* fclose(fopen( shrinkerfile_name, "w" )); */

	  for (i = 0; i < results_collected; ++i)
  	    {
  	      print_profile( results[i], fevol_shrinker );
	      /* fprintf(common_data,"%.2f   %i   %.15G\n", */
	      /* 	      k, i, expander_asymptotics_wrapper( results[i] ) ); */
	      /* fprintf(common_data,"%.2f  %.2f  %i   %.15G\n", */
	      /* 	      k, l, i, pow(-1,i)*results[i] ); */
	      /* fprintf(common_data,"%.2f  %.2f  %i   %.15G   %.15G\n", */
	      /* 	      k, l, i, results[i], fevol_shrinker_reverse(results[i],0,NULL,NULL)); */
  	      solve_eigenproblem(results[i], 1, 4, fevol_shrinker_eigenproblem);
	      /* fevol_shrinker(results[i], 1, shrinkerfile_name, NULL ); */
	    }

	  /* fclose(common_data); */

  	}
    }



  return 0;
}
