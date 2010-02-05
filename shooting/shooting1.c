#include "shooting1.h"

double k = 3.;

int
main (void)
{
  double x;
  FILE * f, * eigenfile;
  double results[10];
  double eigen_results[20];
  int i, j, results_collected, eigen_results_collected;
  char filename[40];


  /* double b = bisec ( 100., 0.1, 6., 1.e-14, 0. ); */
  /* printf( "bisect=%.15f\n", b); */

  sprintf( filename, "shrinker_%.5f.dat", k);

  f=fopen (filename,"w");
  fprintf(f,"# k = %.15f",k);
  fclose(f);

  eigenfile = fopen( "eigefile.dat", "w" );

  results_collected = ripper( 1.,
			      1.e10,
			      1.,
			      RIPPER_DENSE1,
			      3,
			      results,
			      0.,
			      fevol_shrinker,
			      NULL );

  for (i = 0; i < results_collected; ++i)
    {
      eigen_results_collected = ripper
	( -pow(10.,i+1),
	  1.e3,
	  1.,
	  RIPPER_LINEAR,
	  10,
	  eigen_results,
	  0.,
	  fevol_shrinker_eigenproblem,
	  (void*)(results + i) );
      for ( j = 0; j < eigen_results_collected; j++ )
      	{
      	  fprintf(eigenfile, "%i %.15f\n", i,  eigen_results[j] );
	}
      fprintf(eigenfile, "\n\n\n");


    fevol_shrinker(results[i], 1, filename, NULL);
    }

  fclose(eigenfile);

  /* for( x=0.; x<100.; x+=1. ) */
  /*   { */
  /*     printf("%.15f\n",x); */
  /*     fprintf(f,"%.15f %.15f\n",x,fevol(100.,x,0)); */
  /*   } */

  /* fevol ( 100., b, 1 ); */
  /* fevol ( 100000., 1., 1 ); */
  /* fevol ( 100./.01, 2.7387531258849548976*.01, 1 ); */
  /* fevol ( 100., 2.7387531258849548976, 1 ); */
  return 0;
}
