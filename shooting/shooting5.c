#include "shooting1.h"


int
main (void)
{
  double results[1000];
  double L=10,A;
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

      /* solve_eigenproblem(3., 0, 4, fevol_ym_shrinker_eigenproblem); */
      for ( L=-10; L<=10; L+=.1 )
      	{
      	  A=fevol_ym_shrinker_eigenproblem (L, 0, NULL, NULL);
      	  printf("%f %f\n",L, A);
      	  /* printf("\n\n\n"); */
      	}
      /* A=fevol_ym_shrinker_eigenproblem (L, 0, NULL, NULL); */
      /* 	  printf("%f %f\n",L, A); */



  return 0;
}
