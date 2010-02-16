#include "shooting2.h"

int
main (void)
{
  char staticfile_name[100];
  FILE * staticfile;

  k=9.;
  l=1.;

  sprintf(staticfile_name, "static_%.2f.dat", k);
  staticfile = fopen( staticfile_name, "w" );
  fclose(staticfile);

  fevol_static(10000., 1, staticfile_name, NULL );

  return 0;
}
