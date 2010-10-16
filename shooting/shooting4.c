#include "shooting3.h"

int
main (void)
{
  /* char file_name[100]; */
  FILE * file = fopen("test.dat", "w");
  double L=2.73875312588499;
  double results1[10], results2[10];
  int num_results;
  double A;
  int i;

  k=3.;
  l=1.;

  /* fevol_shrinker_regularized(2.,1,"test.dat",NULL); */

  /* #pragma omp parallel for */
  for ( A = .001; A < 30.; A*=1.1 )
    {
      /* solve_eigenproblem(0.01*pow(1.5,i), 1, 2, fevol_expander_eigenproblem); */
      printf("%.15G %.15G\n", A, fevol_shrinker_regularized(A,0,NULL,NULL));
      /* printf("%.5E %.5E\n",L,fevol_expander_eigenproblem(L, 0, NULL, &A)); */
      fprintf(file,"%.15G %.15G\n",A,fevol_shrinker_regularized(A, 0, NULL, NULL));
    }

  /* num_results = harvester(3.,3000.,2.,RIPPER_EXP,3,results1,0.,lambda_0_at_given_A,NULL); */
  /* num_results = harvester(3.,3000.,2.,RIPPER_EXP,10,results2,0.,d1_asymptotic_gsl_wrapper,NULL); */
  /* for( i = 0; i < num_results; i++ ) */
  /*   { */
  /*     fprintf(file,"%.10G %.10G\n",results1[i], results2[i]); */
  /*   } */

  /* for ( A=.01; A < 10; A+=.001 ) */
  /*   { */
  /*     printf("%.5E %.5E\n",A,fevol_expander(A,0,NULL,NULL)); */
  /*     fprintf(file, "%.5E %.5E\n",A,fevol_expander(A,0,NULL,NULL)); */
  /*   } */


  fclose(file);


  return 0;
}

double lambda_at_given_A(double A, int z, char * f, void * ptr)
{
  double phony;

  harvester
    ( 1.,
      -1.e10,
      .5,
      RIPPER_LINEAR,
      1,
      &phony,
      0.,
      fevol_expander_eigenproblem,
      (void*)(&A) );

  return phony;
}

double lambda_0_at_given_A(double A, int z, char * f, void * ptr)
{
    return fevol_expander_eigenproblem(0., 0, NULL, (void *)(&A));
}


double asymptotic_gsl_wrapper( double A, void * ptr )
{
  return fevol_expander(A, 0, NULL, NULL);
}

double d1_asymptotic_gsl_wrapper
( double A,
  int i,
  char * c,
  void * ptr )
{
  gsl_function F;
  F.function = asymptotic_gsl_wrapper;
  F.params = NULL;
  double result;
  double abber;

  gsl_deriv_central( &F, A, 1.e-3, &result, &abber);
  return result;
}
