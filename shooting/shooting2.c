#include "shooting2.h"

/* zeroes of B(A) */

int
main (void)
{
  char staticfile_name[100];
  FILE * staticfile;
  double results[10];
  int i;

  k=3.;
  l=1.;

  sprintf(staticfile_name, "static_%.2f.dat", k);
  staticfile = fopen( staticfile_name, "w" );

  i = harvester(0.001,1.e4,1.1,RIPPER_EXP,10,results,0.,fevol_expander,NULL);

  for ( ; i > 0; i-- )
    {
      fprintf(staticfile, "%.5E\n", results[i-1] );
    }

  fclose(staticfile);

  return 0;
}
