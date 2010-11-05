#include "shooting_expander.h"

#define HARVESTER_DATA_DIR "./harvester_data_expander/"
#define PROFILE_FILE_PREFIX "expander"

int
main (void)
{
  double results[1000];
  double A;
  int i, results_collected;
  char expander_file_name[1000];
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
  	    ( 1.e-5,
  	      1.e10,
  	      1.1,
  	      RIPPER_EXP,
  	      1,
  	      results,
  	      PI/2. - 0.573141133043885, /* b_1 */
  	      fevol_expander,
  	      NULL );

  	  sprintf
  	    ( expander_file_name,
  	      HARVESTER_DATA_DIR PROFILE_FILE_PREFIX HARVESTER_DEFAULT_EXTENSION,
  	      k, l);

	  fclose(fopen(expander_file_name,"w"));
	  fevol_expander(results[0], 1, expander_file_name, NULL);
  	}
    }



  return 0;
}
