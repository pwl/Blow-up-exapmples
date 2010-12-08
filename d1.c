#include "derivatives.h"
#include "stdio.h"

#define MAXLEN 10

int main(int argc, char *argv[])
{
  double x[MAXLEN];
  double y[MAXLEN];
  int i,n=0;

  while(scanf("%lf %lf",x+n,y+n)!=EOF) /* read function from stdout */
    n++;			/* count the number of lines*/

  for( i = 0; i < n; i++ )
    /* print the derivatives to stdout */
    printf("%.16G %.16G %.16G\n",x[i],D1(y,x,i,n),D2(y,x,i,n));

  return 0;
}
