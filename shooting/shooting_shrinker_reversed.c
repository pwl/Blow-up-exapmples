#include "shooting_shrinker_reversed.h"

#define HARVESTER_DATA_DIR "./harvester_data_shrinker/"
#define PROFILE_FILE_PREFIX "shrinker_reversed"

int
main (void)
{
  double results[1000];
  double A;
  int i, results_collected;
  char shrinker_file_name[1000];
  char common_file_name[1000];
  FILE * eigenfile;
  FILE * common_data;

  k=3.;
  l=1.;

  for( l = 1.; l <= 1.01; l+=1. )
    {
      for( k = 3.; k <= 3.; k+=1. )
  	{
	  printf(RED1 "searching for solutions to NODE in dimension %.3f with l=%.1f\n" FORMAT_OFF,k,l);

  	  results_collected = harvester
  	    ( PI/2.-1.e-10,
  	      1.e-100,
  	      1./3.,
  	      RIPPER_EXP,
  	      1,
  	      results,
  	      0.,
  	      fevol_shrinker_reverse,
  	      NULL );

  	  sprintf
  	    ( shrinker_file_name,
  	      HARVESTER_DATA_DIR PROFILE_FILE_PREFIX HARVESTER_DEFAULT_EXTENSION,
  	      k, l);

	  fclose(fopen(shrinker_file_name,"w"));
	  fevol_shrinker_reverse(results[0], 1, shrinker_file_name, NULL);

  	}
    }

  return 0;
}

