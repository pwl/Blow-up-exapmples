#include "shooting3.h"

int
main (void)
{
  /* char file_name[100]; */
  /* FILE * file = fopen("test.dat", "w"); */
  double L;
  double A=.1;
  int i;

  k=3.;
  l=1.;

  fevol_expander(5.5,1,"test.dat",NULL);

  /* #pragma omp parallel for */
  /* for ( i = 0; i < 100; i++ ) */
  /*   { */
  /*     solve_eigenproblem(0.01*pow(1.5,i), 1, 2, fevol_expander_eigenproblem); */
  /*     /\* printf("%.5E %.5E\n",L,fevol_expander_eigenproblem(L, 0, NULL, &A)); *\/ */
  /*     /\* fprintf(file,"%.5E %.5E\n",A,fevol_expander_eigenproblem(A, 0, NULL, &l)); *\/ */
  /*   } */

  /* fclose(file); */


  return 0;
}
