#include "shooting3.h"

int
main (void)
{
  /* char file_name[100]; */
  FILE * file = fopen("test.dat", "w");
  double L;
  double A=.1;

  k=3.;
  l=1.;

  /* sprintf(file_name, "static_%.2f.dat", k); */
  /* staticfile = fopen( staticfile_name, "w" ); */
  /* fclose(staticfile); */

  for ( A = .01; A < 10.; A*=1.5 )
    {
      solve_eigenproblem(A, 1, 1, fevol_expander_eigenproblem);
      /* printf("%.5E %.5E\n",L,fevol_expander_eigenproblem(L, 0, NULL, &A)); */
      /* fprintf(file,"%.5E %.5E\n",A,fevol_expander_eigenproblem(A, 0, NULL, &l)); */
    }
  fclose(file);


  return 0;
}
