#include "derivatives.h"

#ifdef D1_3pt
/* obliczanie pierwszej pochodnej */
double D1 ( double * u, double * x, int i, int N )
{
  double du;

  /* for ( i = 0; i < N; i++ ) */
  /*   printf("%i %.15f %.15f\n", i, x[i], u[i]); */

  if( i > 0 && i < N-1 )
    du=
      (u[-1+i]-u[i])/(x[-1+i]-x[i])+
      (-u[-1+i]+u[1+i])/(x[-1+i]-x[1+i])+
      (u[i]-u[1+i])/(x[i]-x[1+i]);
  else if( i == 0 )
    du=
      (u[1]-u[0])/(x[1]-x[0])
      +(u[2]-u[0])/(x[2]-x[0])
      -(u[2]-u[1])/(x[2]-x[1]);
  else if( i == N-1 )
    du=
      (-u[-3+N]+u[-2+N])/(x[-3+N]-x[-2+N])
      +(u[-3+N]-u[-1+N])/(x[-3+N]-x[-1+N])
      +(u[-2+N]-u[-1+N])/(x[-2+N]-x[-1+N]);
  else du = 0.;

  /* printf("D1: i=%i du=%.15f\n", i, du); */
  return du;
}
#endif

#ifdef D2_3pt
/* obliczanie drugiej pochodnej */
double D2 ( double * u, double * x, int i, int N )
{
  if( i > 0 && i < N-1 )
    return ((u[i+1]-u[i])/(x[i+1]-x[i])
	    -(u[i]-u[i-1])/(x[i]-x[i-1]))
      *2./(x[i+1]-x[i-1]);
  else if (i == 0)
    return (2.*
	    ( (u[i]-u[2+i])/(x[i]-x[2+i])
	      +(-u[1+i]+u[2+i])/(x[1+i]-x[2+i]))
	    )/(x[i]-x[1+i]);
  else if (i==N-1)
    return (2.*
	    ( (u[-2+i]-u[i])/(x[-2+i]-x[i])
	      +(-u[-1+i]+u[i])/(x[-1+i]-x[i])
	      )
	    )/(x[-2+i]-x[-1+i]);
  else return 0.;
}
#endif

#ifdef D1_5pt
double D1 ( double * u, double * x, int i, int N )
{
  double du;

  /* for ( i = 0; i < N; i++ ) */
  /*   printf("%i %.15f %.15f\n", i, x[i], u[i]); */

  if( i > 1 && i < N-2 )
    return (u[i]*(-1 - (x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i]) +
           x[-2 + i]*(x[-1 + i] - 2*x[i] + x[1 + i]))*(x[i] - x[2 + i])*pow(x[-2 + i] - x[i],-1)*
         pow(x[-1 + i] - x[i],-1)*pow(x[i] - x[1 + i],-1)) +
     u[-2 + i]*(x[-1 + i] - x[i])*(x[i] - x[1 + i])*pow(x[-2 + i] - x[-1 + i],-1)*
      pow(x[-2 + i] - x[i],-1)*pow(x[-2 + i] - x[1 + i],-1)*pow(x[-2 + i] - x[2 + i],-1)*
      pow(x[i] - x[2 + i],2) - u[-1 + i]*(x[-2 + i] - x[i])*(x[i] - x[1 + i])*
      pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-1 + i] - x[i],-1)*pow(x[-1 + i] - x[1 + i],-1)*
      pow(x[-1 + i] - x[2 + i],-1)*pow(x[i] - x[2 + i],2) +
     u[1 + i]*(-x[-2 + i] + x[i])*(-x[-1 + i] + x[i])*pow(x[-2 + i] - x[1 + i],-1)*
      pow(-x[-1 + i] + x[1 + i],-1)*pow(-x[i] + x[1 + i],-1)*pow(x[i] - x[2 + i],2)*
      pow(x[1 + i] - x[2 + i],-1) + u[2 + i]*(x[-2 + i] - x[i])*(-x[-1 + i] + x[i])*(x[i] - x[1 + i])*
      pow(x[1 + i] - x[2 + i],-1)*pow(-x[-2 + i] + x[2 + i],-1)*pow(-x[-1 + i] + x[2 + i],-1))*
      pow(-x[i] + x[2 + i],-1);

  else if( i == 0 )
    return pow(-x[i] + x[4 + i],-1)*(u[i]*(-1 + (-x[i] + x[4 + i])*
         (-1 + (-2*x[i] + x[1 + i] + x[2 + i])*(-x[i] + x[3 + i])*pow(x[i] - x[1 + i],-1)*
            pow(-x[i] + x[2 + i],-1))*pow(-x[i] + x[3 + i],-1)) +
     u[1 + i]*(x[i] - x[2 + i])*(x[i] - x[3 + i])*pow(x[i] - x[1 + i],-1)*pow(x[1 + i] - x[2 + i],-1)*
      pow(x[1 + i] - x[3 + i],-1)*pow(x[i] - x[4 + i],2)*pow(x[1 + i] - x[4 + i],-1) +
     u[3 + i]*(x[i] - x[1 + i])*(x[i] - x[2 + i])*pow(x[2 + i] - x[3 + i],-1)*pow(-x[i] + x[3 + i],-1)*
      pow(-x[1 + i] + x[3 + i],-1)*pow(x[i] - x[4 + i],2)*pow(x[3 + i] - x[4 + i],-1) +
     u[2 + i]*(x[i] - x[1 + i])*(-x[i] + x[3 + i])*pow(-x[i] + x[2 + i],-1)*pow(-x[1 + i] + x[2 + i],-1)*
      pow(-x[2 + i] + x[3 + i],-1)*pow(x[i] - x[4 + i],2)*pow(-x[2 + i] + x[4 + i],-1) +
     u[4 + i]*(x[i] - x[1 + i])*(x[i] - x[2 + i])*(x[i] - x[3 + i])*pow(-x[1 + i] + x[4 + i],-1)*
				    pow(-x[2 + i] + x[4 + i],-1)*pow(-x[3 + i] + x[4 + i],-1));
  else if ( i == 1 )
    return pow(-x[i] + x[3 + i],-1)*(u[i]*(-1 + (-x[i] + x[3 + i])*
         (-1 - (x[-1 + i] - 2*x[i] + x[1 + i])*(x[i] - x[2 + i])*pow(x[-1 + i] - x[i],-1)*
            pow(x[i] - x[1 + i],-1))*pow(-x[i] + x[2 + i],-1)) -
     u[-1 + i]*(x[i] - x[1 + i])*(x[i] - x[2 + i])*pow(x[-1 + i] - x[i],-1)*pow(x[-1 + i] - x[1 + i],-1)*
      pow(x[-1 + i] - x[2 + i],-1)*pow(x[-1 + i] - x[3 + i],-1)*pow(x[i] - x[3 + i],2) +
     u[1 + i]*(x[-1 + i] - x[i])*(x[i] - x[2 + i])*pow(x[-1 + i] - x[1 + i],-1)*pow(x[i] - x[1 + i],-1)*
      pow(x[1 + i] - x[2 + i],-1)*pow(x[i] - x[3 + i],2)*pow(x[1 + i] - x[3 + i],-1) +
     u[2 + i]*(-x[-1 + i] + x[i])*(x[i] - x[1 + i])*pow(x[-1 + i] - x[2 + i],-1)*
      pow(-x[i] + x[2 + i],-1)*pow(-x[1 + i] + x[2 + i],-1)*pow(x[i] - x[3 + i],2)*
      pow(x[2 + i] - x[3 + i],-1) + u[3 + i]*(x[-1 + i] - x[i])*(x[i] - x[1 + i])*(x[i] - x[2 + i])*
				pow(x[2 + i] - x[3 + i],-1)*pow(-x[-1 + i] + x[3 + i],-1)*pow(-x[1 + i] + x[3 + i],-1));
  else if ( i == N-2 )
    return (-(u[i]*(x[-2 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i])) +
          x[-3 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i]) +
             x[-2 + i]*(x[-1 + i] - 2*x[i] + x[1 + i])) +
          x[i]*(x[-1 + i]*(3*x[i] - 2*x[1 + i]) + x[i]*(-4*x[i] + 3*x[1 + i])))*pow(x[-2 + i] - x[i],-1)*
        pow(-x[-3 + i] + x[i],-1)*pow(-x[-1 + i] + x[i],-1)) +
     u[-3 + i]*(x[-2 + i] - x[i])*(-x[-1 + i] + x[i])*pow(x[-3 + i] - x[-2 + i],-1)*
      pow(x[-3 + i] - x[-1 + i],-1)*pow(x[-3 + i] - x[i],-1)*pow(x[-3 + i] - x[1 + i],-1)*
      pow(x[i] - x[1 + i],2) + u[-2 + i]*(x[-3 + i] - x[i])*(x[-1 + i] - x[i])*
      pow(x[-3 + i] - x[-2 + i],-1)*pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-2 + i] - x[i],-1)*
      pow(x[-2 + i] - x[1 + i],-1)*pow(x[i] - x[1 + i],2) +
     u[-1 + i]*(x[-3 + i] - x[i])*(-x[-2 + i] + x[i])*pow(x[-3 + i] - x[-1 + i],-1)*
      pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-1 + i] - x[i],-1)*pow(x[-1 + i] - x[1 + i],-1)*
      pow(x[i] - x[1 + i],2) + u[1 + i]*(x[-3 + i] - x[i])*(-x[-2 + i] + x[i])*(-x[-1 + i] + x[i])*
      pow(x[-1 + i] - x[1 + i],-1)*pow(-x[-3 + i] + x[1 + i],-1)*pow(-x[-2 + i] + x[1 + i],-1))*
	pow(-x[i] + x[1 + i],-1);
  else if( i == N-1 )
    return pow(x[-2 + i] - x[i],-1)*pow(-x[-4 + i] + x[i],-1)*
   (u[-2 + i]*(x[-3 + i] - x[i])*(x[-1 + i] - x[i])*pow(x[-4 + i] - x[-2 + i],-1)*
      pow(x[-3 + i] - x[-2 + i],-1)*pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-4 + i] - x[i],2) +
     u[-4 + i]*(x[-3 + i] - x[i])*(x[-1 + i] - x[i])*pow(x[-4 + i] - x[-3 + i],-1)*
      pow(x[-4 + i] - x[-2 + i],-1)*pow(x[-4 + i] - x[-1 + i],-1)*pow(x[-2 + i] - x[i],2) +
     u[-3 + i]*(-x[-1 + i] + x[i])*pow(x[-4 + i] - x[-3 + i],-1)*pow(x[-3 + i] - x[-2 + i],-1)*
      pow(x[-3 + i] - x[-1 + i],-1)*pow(x[-4 + i] - x[i],2)*pow(x[-3 + i] - x[i],-1)*
      pow(x[-2 + i] - x[i],2) + u[-1 + i]*(-x[-3 + i] + x[i])*pow(x[-3 + i] - x[-1 + i],-1)*
      pow(-x[-4 + i] + x[-1 + i],-1)*pow(-x[-2 + i] + x[-1 + i],-1)*pow(x[-4 + i] - x[i],2)*
      pow(x[-2 + i] - x[i],2)*pow(x[-1 + i] - x[i],-1) +
     u[i]*(x[i]*((3*x[-1 + i] - 4*x[i])*x[i] + x[-2 + i]*(-2*x[-1 + i] + 3*x[i])) +
        x[-3 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) + x[i]*(-2*x[-1 + i] + 3*x[i])) +
        x[-4 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) + x[-3 + i]*(x[-2 + i] + x[-1 + i] - 2*x[i]) +
		   x[i]*(-2*x[-1 + i] + 3*x[i])))*pow(-x[-3 + i] + x[i],-1)*pow(-x[-1 + i] + x[i],-1));
  return 0.;
}
#endif

#ifdef D1_5pt_6pt
double D1 ( double * u, double * x, int i, int N )
{
  double du;

  /* for ( i = 0; i < N; i++ ) */
  /*   printf("%i %.15f %.15f\n", i, x[i], u[i]); */

  if( i > 1 && i < N-2 )
    return (u[i]*(-1 - (x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i]) +
           x[-2 + i]*(x[-1 + i] - 2*x[i] + x[1 + i]))*(x[i] - x[2 + i])*pow(x[-2 + i] - x[i],-1)*
         pow(x[-1 + i] - x[i],-1)*pow(x[i] - x[1 + i],-1)) +
     u[-2 + i]*(x[-1 + i] - x[i])*(x[i] - x[1 + i])*pow(x[-2 + i] - x[-1 + i],-1)*
      pow(x[-2 + i] - x[i],-1)*pow(x[-2 + i] - x[1 + i],-1)*pow(x[-2 + i] - x[2 + i],-1)*
      pow(x[i] - x[2 + i],2) - u[-1 + i]*(x[-2 + i] - x[i])*(x[i] - x[1 + i])*
      pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-1 + i] - x[i],-1)*pow(x[-1 + i] - x[1 + i],-1)*
      pow(x[-1 + i] - x[2 + i],-1)*pow(x[i] - x[2 + i],2) +
     u[1 + i]*(-x[-2 + i] + x[i])*(-x[-1 + i] + x[i])*pow(x[-2 + i] - x[1 + i],-1)*
      pow(-x[-1 + i] + x[1 + i],-1)*pow(-x[i] + x[1 + i],-1)*pow(x[i] - x[2 + i],2)*
      pow(x[1 + i] - x[2 + i],-1) + u[2 + i]*(x[-2 + i] - x[i])*(-x[-1 + i] + x[i])*(x[i] - x[1 + i])*
      pow(x[1 + i] - x[2 + i],-1)*pow(-x[-2 + i] + x[2 + i],-1)*pow(-x[-1 + i] + x[2 + i],-1))*
      pow(-x[i] + x[2 + i],-1);

  else if( i == 0 )
    return pow(-x[i] + x[5 + i],-1)*(u[i]*
      (-1 + (-x[i] + x[5 + i])*
         (-1 + (-x[i] + x[4 + i])*
            (-1 + (-2*x[i] + x[1 + i] + x[2 + i])*
               (-x[i] + x[3 + i])*pow(x[i] - x[1 + i],-1)*
               pow(-x[i] + x[2 + i],-1))*pow(-x[i] + x[3 + i],-1))
          *pow(-x[i] + x[4 + i],-1)) +
     u[1 + i]*(x[i] - x[2 + i])*(x[i] - x[3 + i])*
      (x[i] - x[4 + i])*pow(x[i] - x[1 + i],-1)*
      pow(x[1 + i] - x[2 + i],-1)*pow(x[1 + i] - x[3 + i],-1)*
      pow(x[1 + i] - x[4 + i],-1)*pow(x[i] - x[5 + i],2)*
      pow(x[1 + i] - x[5 + i],-1) +
     u[2 + i]*(x[i] - x[1 + i])*(-x[i] + x[3 + i])*
      (-x[i] + x[4 + i])*pow(x[i] - x[2 + i],-1)*
      pow(-x[1 + i] + x[2 + i],-1)*pow(x[2 + i] - x[3 + i],-1)*
      pow(x[2 + i] - x[4 + i],-1)*pow(x[i] - x[5 + i],2)*
      pow(x[2 + i] - x[5 + i],-1) +
     u[3 + i]*(x[i] - x[1 + i])*(x[i] - x[2 + i])*
      (x[i] - x[4 + i])*pow(x[2 + i] - x[3 + i],-1)*
      pow(-x[i] + x[3 + i],-1)*pow(-x[1 + i] + x[3 + i],-1)*
      pow(x[3 + i] - x[4 + i],-1)*pow(x[i] - x[5 + i],2)*
      pow(x[3 + i] - x[5 + i],-1) +
     u[4 + i]*(x[i] - x[1 + i])*(x[i] - x[2 + i])*
      (x[i] - x[3 + i])*pow(x[2 + i] - x[4 + i],-1)*
      pow(-x[i] + x[4 + i],-1)*pow(-x[1 + i] + x[4 + i],-1)*
      pow(-x[3 + i] + x[4 + i],-1)*pow(x[i] - x[5 + i],2)*
      pow(x[4 + i] - x[5 + i],-1) +
     u[5 + i]*(x[i] - x[1 + i])*(x[i] - x[2 + i])*
      (x[i] - x[3 + i])*(x[i] - x[4 + i])*
      pow(-x[1 + i] + x[5 + i],-1)*pow(-x[2 + i] + x[5 + i],-1)*
				     pow(-x[3 + i] + x[5 + i],-1)*pow(-x[4 + i] + x[5 + i],-1));
  else if ( i == 1 )
    return pow(-x[i] + x[3 + i],-1)*(u[i]*(-1 + (-x[i] + x[3 + i])*
         (-1 - (x[-1 + i] - 2*x[i] + x[1 + i])*(x[i] - x[2 + i])*pow(x[-1 + i] - x[i],-1)*
            pow(x[i] - x[1 + i],-1))*pow(-x[i] + x[2 + i],-1)) -
     u[-1 + i]*(x[i] - x[1 + i])*(x[i] - x[2 + i])*pow(x[-1 + i] - x[i],-1)*pow(x[-1 + i] - x[1 + i],-1)*
      pow(x[-1 + i] - x[2 + i],-1)*pow(x[-1 + i] - x[3 + i],-1)*pow(x[i] - x[3 + i],2) +
     u[1 + i]*(x[-1 + i] - x[i])*(x[i] - x[2 + i])*pow(x[-1 + i] - x[1 + i],-1)*pow(x[i] - x[1 + i],-1)*
      pow(x[1 + i] - x[2 + i],-1)*pow(x[i] - x[3 + i],2)*pow(x[1 + i] - x[3 + i],-1) +
     u[2 + i]*(-x[-1 + i] + x[i])*(x[i] - x[1 + i])*pow(x[-1 + i] - x[2 + i],-1)*
      pow(-x[i] + x[2 + i],-1)*pow(-x[1 + i] + x[2 + i],-1)*pow(x[i] - x[3 + i],2)*
      pow(x[2 + i] - x[3 + i],-1) + u[3 + i]*(x[-1 + i] - x[i])*(x[i] - x[1 + i])*(x[i] - x[2 + i])*
				pow(x[2 + i] - x[3 + i],-1)*pow(-x[-1 + i] + x[3 + i],-1)*pow(-x[1 + i] + x[3 + i],-1));
  else if ( i == N-2 )
    return (-(u[i]*(x[-2 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i])) +
          x[-3 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i]) +
             x[-2 + i]*(x[-1 + i] - 2*x[i] + x[1 + i])) +
          x[i]*(x[-1 + i]*(3*x[i] - 2*x[1 + i]) + x[i]*(-4*x[i] + 3*x[1 + i])))*pow(x[-2 + i] - x[i],-1)*
        pow(-x[-3 + i] + x[i],-1)*pow(-x[-1 + i] + x[i],-1)) +
     u[-3 + i]*(x[-2 + i] - x[i])*(-x[-1 + i] + x[i])*pow(x[-3 + i] - x[-2 + i],-1)*
      pow(x[-3 + i] - x[-1 + i],-1)*pow(x[-3 + i] - x[i],-1)*pow(x[-3 + i] - x[1 + i],-1)*
      pow(x[i] - x[1 + i],2) + u[-2 + i]*(x[-3 + i] - x[i])*(x[-1 + i] - x[i])*
      pow(x[-3 + i] - x[-2 + i],-1)*pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-2 + i] - x[i],-1)*
      pow(x[-2 + i] - x[1 + i],-1)*pow(x[i] - x[1 + i],2) +
     u[-1 + i]*(x[-3 + i] - x[i])*(-x[-2 + i] + x[i])*pow(x[-3 + i] - x[-1 + i],-1)*
      pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-1 + i] - x[i],-1)*pow(x[-1 + i] - x[1 + i],-1)*
      pow(x[i] - x[1 + i],2) + u[1 + i]*(x[-3 + i] - x[i])*(-x[-2 + i] + x[i])*(-x[-1 + i] + x[i])*
      pow(x[-1 + i] - x[1 + i],-1)*pow(-x[-3 + i] + x[1 + i],-1)*pow(-x[-2 + i] + x[1 + i],-1))*
	pow(-x[i] + x[1 + i],-1);
  else if( i == N-1 )
    return pow(-x[-5 + i] + x[i],-1)*(u[-5 + i]*(x[-4 + i] - x[i])*
      (-x[-3 + i] + x[i])*(-x[-2 + i] + x[i])*(-x[-1 + i] + x[i])*
      pow(x[-5 + i] - x[-4 + i],-1)*pow(x[-5 + i] - x[-3 + i],-1)*
      pow(x[-5 + i] - x[-2 + i],-1)*pow(x[-5 + i] - x[-1 + i],-1)\
      + u[-4 + i]*(x[-3 + i] - x[i])*(x[-2 + i] - x[i])*
      (x[-1 + i] - x[i])*pow(x[-5 + i] - x[-4 + i],-1)*
      pow(x[-4 + i] - x[-3 + i],-1)*pow(x[-4 + i] - x[-2 + i],-1)*
      pow(x[-4 + i] - x[-1 + i],-1)*pow(x[-5 + i] - x[i],2)*
      pow(x[-4 + i] - x[i],-1) +
     u[-3 + i]*(-x[-4 + i] + x[i])*(-x[-2 + i] + x[i])*
      (-x[-1 + i] + x[i])*pow(x[-5 + i] - x[-3 + i],-1)*
      pow(x[-4 + i] - x[-3 + i],-1)*pow(x[-3 + i] - x[-2 + i],-1)*
      pow(x[-3 + i] - x[-1 + i],-1)*pow(x[-5 + i] - x[i],2)*
      pow(x[-3 + i] - x[i],-1) +
     u[-2 + i]*(x[-4 + i] - x[i])*(x[-3 + i] - x[i])*
      (x[-1 + i] - x[i])*pow(x[-4 + i] - x[-2 + i],-1)*
      pow(-x[-5 + i] + x[-2 + i],-1)*
      pow(-x[-3 + i] + x[-2 + i],-1)*
      pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-5 + i] - x[i],2)*
      pow(x[-2 + i] - x[i],-1) +
     u[-1 + i]*(x[-4 + i] - x[i])*(x[-3 + i] - x[i])*
      (x[-2 + i] - x[i])*pow(x[-4 + i] - x[-1 + i],-1)*
      pow(-x[-5 + i] + x[-1 + i],-1)*
      pow(-x[-3 + i] + x[-1 + i],-1)*
      pow(-x[-2 + i] + x[-1 + i],-1)*pow(x[-5 + i] - x[i],2)*
      pow(x[-1 + i] - x[i],-1) -
     u[i]*(x[-4 + i]*(x[i]*
            ((3*x[-1 + i] - 4*x[i])*x[i] +
              x[-2 + i]*(-2*x[-1 + i] + 3*x[i])) +
           x[-3 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) +
              x[i]*(-2*x[-1 + i] + 3*x[i]))) +
        x[-5 + i]*(x[i]*((3*x[-1 + i] - 4*x[i])*x[i] +
              x[-2 + i]*(-2*x[-1 + i] + 3*x[i])) +
           x[-3 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) +
              x[i]*(-2*x[-1 + i] + 3*x[i])) +
           x[-4 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) +
              x[-3 + i]*(x[-2 + i] + x[-1 + i] - 2*x[i]) +
              x[i]*(-2*x[-1 + i] + 3*x[i]))) +
        x[i]*(x[-3 + i]*((3*x[-1 + i] - 4*x[i])*x[i] +
              x[-2 + i]*(-2*x[-1 + i] + 3*x[i])) +
           x[i]*(x[-2 + i]*(3*x[-1 + i] - 4*x[i]) +
              x[i]*(-4*x[-1 + i] + 5*x[i]))))*
      pow(x[-3 + i] - x[i],-1)*pow(-x[-4 + i] + x[i],-1)*
      pow(-x[-2 + i] + x[i],-1)*pow(-x[-1 + i] + x[i],-1));
  return 0.;
}
#endif

#ifdef D2_5pt
/* obliczanie drugiej pochodnej */
double D2 ( double * u, double * x, int i, int N )
{
  if( i > 1 && i < N-2 )
    return 2*(u[-2 + i]*pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-2 + i] - x[i],-1)*
      (x[1 + i]*x[2 + i] - 2*x[i]*(x[1 + i] + x[2 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i] + x[2 + i]) +
        3*pow(x[i],2))*pow(x[-2 + i] - x[1 + i],-1)*pow(x[-2 + i] - x[2 + i],-1) +
     u[-1 + i]*pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-1 + i] - x[i],-1)*
      (x[-2 + i]*(2*x[i] - x[1 + i] - x[2 + i]) - x[1 + i]*x[2 + i] + 2*x[i]*(x[1 + i] + x[2 + i]) -
        3*pow(x[i],2))*pow(x[-1 + i] - x[1 + i],-1)*pow(x[-1 + i] - x[2 + i],-1) +
     u[i]*pow(x[-2 + i] - x[i],-1)*pow(x[-1 + i] - x[i],-1)*
      (-3*x[i]*x[1 + i] - 3*x[i]*x[2 + i] + x[1 + i]*x[2 + i] +
        x[-1 + i]*(-3*x[i] + x[1 + i] + x[2 + i]) +
        x[-2 + i]*(x[-1 + i] - 3*x[i] + x[1 + i] + x[2 + i]) + 6*pow(x[i],2))*pow(x[i] - x[1 + i],-1)*
      pow(x[i] - x[2 + i],-1) - u[1 + i]*(x[i]*(3*x[i] - 2*x[2 + i]) + x[-1 + i]*(-2*x[i] + x[2 + i]) +
        x[-2 + i]*(x[-1 + i] - 2*x[i] + x[2 + i]))*pow(x[-1 + i] - x[1 + i],-1)*
      pow(-x[-2 + i] + x[1 + i],-1)*pow(-x[i] + x[1 + i],-1)*pow(x[1 + i] - x[2 + i],-1) -
     u[2 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i]) +
        x[-2 + i]*(x[-1 + i] - 2*x[i] + x[1 + i]))*pow(x[-1 + i] - x[2 + i],-1)*
	      pow(-x[-2 + i] + x[2 + i],-1)*pow(-x[i] + x[2 + i],-1)*pow(-x[1 + i] + x[2 + i],-1));
  else if (i == 0)
    return 2*u[i]*(x[2 + i]*x[3 + i] + x[2 + i]*x[4 + i] + x[3 + i]*x[4 + i] +
      x[1 + i]*(x[2 + i] + x[3 + i] + x[4 + i]) - 3*x[i]*(x[1 + i] + x[2 + i] + x[3 + i] + x[4 + i]) +
      6*pow(x[i],2))*pow(x[i] - x[1 + i],-1)*pow(x[i] - x[2 + i],-1)*pow(x[i] - x[3 + i],-1)*
    pow(x[i] - x[4 + i],-1) - 2*u[1 + i]*(x[3 + i]*x[4 + i] + x[2 + i]*(x[3 + i] + x[4 + i]) -
      2*x[i]*(x[2 + i] + x[3 + i] + x[4 + i]) + 3*pow(x[i],2))*pow(x[i] - x[1 + i],-1)*
    pow(x[1 + i] - x[2 + i],-1)*pow(x[1 + i] - x[3 + i],-1)*pow(x[1 + i] - x[4 + i],-1) +
   2*u[2 + i]*(x[3 + i]*x[4 + i] + x[1 + i]*(x[3 + i] + x[4 + i]) -
      2*x[i]*(x[1 + i] + x[3 + i] + x[4 + i]) + 3*pow(x[i],2))*pow(x[i] - x[2 + i],-1)*
    pow(x[1 + i] - x[2 + i],-1)*pow(x[2 + i] - x[3 + i],-1)*pow(x[2 + i] - x[4 + i],-1) -
   2*u[3 + i]*(x[2 + i]*x[4 + i] + x[1 + i]*(x[2 + i] + x[4 + i]) -
      2*x[i]*(x[1 + i] + x[2 + i] + x[4 + i]) + 3*pow(x[i],2))*pow(x[1 + i] - x[3 + i],-1)*
    pow(-x[i] + x[3 + i],-1)*pow(-x[2 + i] + x[3 + i],-1)*pow(x[3 + i] - x[4 + i],-1) -
   2*u[4 + i]*(x[2 + i]*x[3 + i] + x[1 + i]*(x[2 + i] + x[3 + i]) -
      2*x[i]*(x[1 + i] + x[2 + i] + x[3 + i]) + 3*pow(x[i],2))*pow(x[1 + i] - x[4 + i],-1)*
      pow(-x[i] + x[4 + i],-1)*pow(-x[2 + i] + x[4 + i],-1)*pow(-x[3 + i] + x[4 + i],-1);
  else if (i==1)
    return  2*(u[-1 + i]*pow(x[-1 + i] - x[i],-1)*(x[2 + i]*x[3 + i] + x[1 + i]*(x[2 + i] + x[3 + i]) -
        2*x[i]*(x[1 + i] + x[2 + i] + x[3 + i]) + 3*pow(x[i],2))*pow(x[-1 + i] - x[1 + i],-1)*
      pow(x[-1 + i] - x[2 + i],-1)*pow(x[-1 + i] - x[3 + i],-1) +
     u[i]*pow(x[-1 + i] - x[i],-1)*(-(x[1 + i]*x[2 + i]) +
        x[-1 + i]*(3*x[i] - x[1 + i] - x[2 + i] - x[3 + i]) - x[1 + i]*x[3 + i] - x[2 + i]*x[3 + i] +
        3*x[i]*(x[1 + i] + x[2 + i] + x[3 + i]) - 6*pow(x[i],2))*pow(x[i] - x[1 + i],-1)*
      pow(x[i] - x[2 + i],-1)*pow(x[i] - x[3 + i],-1) +
     u[1 + i]*(x[2 + i]*x[3 + i] - 2*x[i]*(x[2 + i] + x[3 + i]) +
        x[-1 + i]*(-2*x[i] + x[2 + i] + x[3 + i]) + 3*pow(x[i],2))*pow(x[-1 + i] - x[1 + i],-1)*
      pow(x[i] - x[1 + i],-1)*pow(x[1 + i] - x[2 + i],-1)*pow(x[1 + i] - x[3 + i],-1) -
     u[2 + i]*(x[1 + i]*x[3 + i] - 2*x[i]*(x[1 + i] + x[3 + i]) +
        x[-1 + i]*(-2*x[i] + x[1 + i] + x[3 + i]) + 3*pow(x[i],2))*pow(x[i] - x[2 + i],-1)*
      pow(-x[-1 + i] + x[2 + i],-1)*pow(-x[1 + i] + x[2 + i],-1)*pow(x[2 + i] - x[3 + i],-1) -
     u[3 + i]*(x[1 + i]*x[2 + i] - 2*x[i]*(x[1 + i] + x[2 + i]) +
        x[-1 + i]*(-2*x[i] + x[1 + i] + x[2 + i]) + 3*pow(x[i],2))*pow(x[1 + i] - x[3 + i],-1)*
	       pow(-x[-1 + i] + x[3 + i],-1)*pow(-x[i] + x[3 + i],-1)*pow(-x[2 + i] + x[3 + i],-1));
  else if (i==N-2)
    return 2*u[-3 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i]) +
      x[-2 + i]*(x[-1 + i] - 2*x[i] + x[1 + i]))*pow(x[-3 + i] - x[-2 + i],-1)*
    pow(x[-3 + i] - x[-1 + i],-1)*pow(x[-3 + i] - x[i],-1)*pow(x[-3 + i] - x[1 + i],-1) -
   2*u[-2 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i]) +
      x[-3 + i]*(x[-1 + i] - 2*x[i] + x[1 + i]))*pow(x[-3 + i] - x[-2 + i],-1)*
    pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-2 + i] - x[i],-1)*pow(x[-2 + i] - x[1 + i],-1) +
   2*u[-1 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-2 + i]*(-2*x[i] + x[1 + i]) +
      x[-3 + i]*(x[-2 + i] - 2*x[i] + x[1 + i]))*pow(x[-3 + i] - x[-1 + i],-1)*
    pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-1 + i] - x[i],-1)*pow(x[-1 + i] - x[1 + i],-1) -
   2*u[i]*pow(x[-2 + i] - x[i],-1)*(-3*x[-1 + i]*x[i] + x[-1 + i]*x[1 + i] - 3*x[i]*x[1 + i] +
      x[-2 + i]*(x[-1 + i] - 3*x[i] + x[1 + i]) +
      x[-3 + i]*(x[-2 + i] + x[-1 + i] - 3*x[i] + x[1 + i]) + 6*pow(x[i],2))*pow(-x[-3 + i] + x[i],-1)*
    pow(-x[-1 + i] + x[i],-1)*pow(x[i] - x[1 + i],-1) -
   2*u[1 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) + x[-3 + i]*(x[-2 + i] + x[-1 + i] - 2*x[i]) +
      x[i]*(-2*x[-1 + i] + 3*x[i]))*pow(x[-2 + i] - x[1 + i],-1)*pow(-x[-3 + i] + x[1 + i],-1)*
      pow(-x[-1 + i] + x[1 + i],-1)*pow(-x[i] + x[1 + i],-1);

  else if (i==N-1)
    return 2*u[-4 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) + x[-3 + i]*(x[-2 + i] + x[-1 + i] - 2*x[i]) +
      x[i]*(-2*x[-1 + i] + 3*x[i]))*pow(x[-4 + i] - x[-3 + i],-1)*pow(x[-4 + i] - x[-2 + i],-1)*
    pow(x[-4 + i] - x[-1 + i],-1)*pow(x[-4 + i] - x[i],-1) -
   2*u[-3 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) + x[-4 + i]*(x[-2 + i] + x[-1 + i] - 2*x[i]) +
      x[i]*(-2*x[-1 + i] + 3*x[i]))*pow(x[-4 + i] - x[-3 + i],-1)*pow(x[-3 + i] - x[-2 + i],-1)*
    pow(x[-3 + i] - x[-1 + i],-1)*pow(x[-3 + i] - x[i],-1) +
   2*u[-2 + i]*(x[-3 + i]*(x[-1 + i] - 2*x[i]) + x[-4 + i]*(x[-3 + i] + x[-1 + i] - 2*x[i]) +
      x[i]*(-2*x[-1 + i] + 3*x[i]))*pow(x[-4 + i] - x[-2 + i],-1)*pow(x[-3 + i] - x[-2 + i],-1)*
    pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-2 + i] - x[i],-1) -
   2*u[-1 + i]*(x[-3 + i]*(x[-2 + i] - 2*x[i]) + x[-4 + i]*(x[-3 + i] + x[-2 + i] - 2*x[i]) +
      x[i]*(-2*x[-2 + i] + 3*x[i]))*pow(x[-3 + i] - x[-1 + i],-1)*pow(-x[-4 + i] + x[-1 + i],-1)*
    pow(-x[-2 + i] + x[-1 + i],-1)*pow(x[-1 + i] - x[i],-1) -
   2*u[i]*pow(x[-3 + i] - x[i],-1)*(x[-2 + i]*x[-1 + i] + x[-3 + i]*(x[-2 + i] + x[-1 + i] - 3*x[i]) +
      x[-4 + i]*(x[-3 + i] + x[-2 + i] + x[-1 + i] - 3*x[i]) - 3*x[-2 + i]*x[i] - 3*x[-1 + i]*x[i] +
				    6*pow(x[i],2))*pow(-x[-4 + i] + x[i],-1)*pow(-x[-2 + i] + x[i],-1)*pow(-x[-1 + i] + x[i],-1);
  else return 0.;
}
#endif

#ifdef D2_5pt_6pt
/* obliczanie drugiej pochodnej */
double D2 ( double * u, double * x, int i, int N )
{
  if( i > 1 && i < N-2 )
    return 2*(u[-2 + i]*pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-2 + i] - x[i],-1)*
      (x[1 + i]*x[2 + i] - 2*x[i]*(x[1 + i] + x[2 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i] + x[2 + i]) +
        3*pow(x[i],2))*pow(x[-2 + i] - x[1 + i],-1)*pow(x[-2 + i] - x[2 + i],-1) +
     u[-1 + i]*pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-1 + i] - x[i],-1)*
      (x[-2 + i]*(2*x[i] - x[1 + i] - x[2 + i]) - x[1 + i]*x[2 + i] + 2*x[i]*(x[1 + i] + x[2 + i]) -
        3*pow(x[i],2))*pow(x[-1 + i] - x[1 + i],-1)*pow(x[-1 + i] - x[2 + i],-1) +
     u[i]*pow(x[-2 + i] - x[i],-1)*pow(x[-1 + i] - x[i],-1)*
      (-3*x[i]*x[1 + i] - 3*x[i]*x[2 + i] + x[1 + i]*x[2 + i] +
        x[-1 + i]*(-3*x[i] + x[1 + i] + x[2 + i]) +
        x[-2 + i]*(x[-1 + i] - 3*x[i] + x[1 + i] + x[2 + i]) + 6*pow(x[i],2))*pow(x[i] - x[1 + i],-1)*
      pow(x[i] - x[2 + i],-1) - u[1 + i]*(x[i]*(3*x[i] - 2*x[2 + i]) + x[-1 + i]*(-2*x[i] + x[2 + i]) +
        x[-2 + i]*(x[-1 + i] - 2*x[i] + x[2 + i]))*pow(x[-1 + i] - x[1 + i],-1)*
      pow(-x[-2 + i] + x[1 + i],-1)*pow(-x[i] + x[1 + i],-1)*pow(x[1 + i] - x[2 + i],-1) -
     u[2 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i]) +
        x[-2 + i]*(x[-1 + i] - 2*x[i] + x[1 + i]))*pow(x[-1 + i] - x[2 + i],-1)*
	      pow(-x[-2 + i] + x[2 + i],-1)*pow(-x[i] + x[2 + i],-1)*pow(-x[1 + i] + x[2 + i],-1));
  else if (i == 0)
    return 2*(u[i]*(-(x[2 + i]*x[3 + i]*x[4 + i]) -
        x[2 + i]*x[3 + i]*x[5 + i] - x[2 + i]*x[4 + i]*x[5 + i] -
        x[3 + i]*x[4 + i]*x[5 + i] -
        x[1 + i]*(x[4 + i]*x[5 + i] +
           x[3 + i]*(x[4 + i] + x[5 + i]) +
           x[2 + i]*(x[3 + i] + x[4 + i] + x[5 + i])) +
        3*x[i]*(x[3 + i]*x[4 + i] + x[3 + i]*x[5 + i] +
           x[4 + i]*x[5 + i] +
           x[2 + i]*(x[3 + i] + x[4 + i] + x[5 + i]) +
           x[1 + i]*(x[2 + i] + x[3 + i] + x[4 + i] + x[5 + i]))\
         - 6*(x[1 + i] + x[2 + i] + x[3 + i] + x[4 + i] +
           x[5 + i])*pow(x[i],2) + 10*pow(x[i],3))*
      pow(x[i] - x[1 + i],-1)*pow(x[i] - x[2 + i],-1)*
      pow(x[i] - x[3 + i],-1)*pow(x[i] - x[4 + i],-1)*
      pow(x[i] - x[5 + i],-1) -
     u[1 + i]*(-(x[3 + i]*x[4 + i]*x[5 + i]) -
        x[2 + i]*(x[4 + i]*x[5 + i] +
           x[3 + i]*(x[4 + i] + x[5 + i])) +
        2*x[i]*(x[4 + i]*x[5 + i] +
           x[3 + i]*(x[4 + i] + x[5 + i]) +
           x[2 + i]*(x[3 + i] + x[4 + i] + x[5 + i])) -
        3*(x[2 + i] + x[3 + i] + x[4 + i] + x[5 + i])*
         pow(x[i],2) + 4*pow(x[i],3))*pow(x[i] - x[1 + i],-1)*
      pow(x[1 + i] - x[2 + i],-1)*pow(x[1 + i] - x[3 + i],-1)*
      pow(x[1 + i] - x[4 + i],-1)*pow(x[1 + i] - x[5 + i],-1) +
     u[2 + i]*(-(x[3 + i]*x[4 + i]*x[5 + i]) -
        x[1 + i]*(x[4 + i]*x[5 + i] +
           x[3 + i]*(x[4 + i] + x[5 + i])) +
        2*x[i]*(x[4 + i]*x[5 + i] +
           x[3 + i]*(x[4 + i] + x[5 + i]) +
           x[1 + i]*(x[3 + i] + x[4 + i] + x[5 + i])) -
        3*(x[1 + i] + x[3 + i] + x[4 + i] + x[5 + i])*
         pow(x[i],2) + 4*pow(x[i],3))*pow(x[i] - x[2 + i],-1)*
      pow(x[1 + i] - x[2 + i],-1)*pow(x[2 + i] - x[3 + i],-1)*
      pow(x[2 + i] - x[4 + i],-1)*pow(x[2 + i] - x[5 + i],-1) +
     u[3 + i]*(x[2 + i]*x[4 + i]*x[5 + i] +
        x[1 + i]*(x[4 + i]*x[5 + i] +
           x[2 + i]*(x[4 + i] + x[5 + i])) -
        2*x[i]*(x[4 + i]*x[5 + i] +
           x[2 + i]*(x[4 + i] + x[5 + i]) +
           x[1 + i]*(x[2 + i] + x[4 + i] + x[5 + i])) +
        3*(x[1 + i] + x[2 + i] + x[4 + i] + x[5 + i])*
         pow(x[i],2) - 4*pow(x[i],3))*pow(x[1 + i] - x[3 + i],-1)*
      pow(-x[i] + x[3 + i],-1)*pow(-x[2 + i] + x[3 + i],-1)*
      pow(x[3 + i] - x[4 + i],-1)*pow(x[3 + i] - x[5 + i],-1) +
     u[4 + i]*(x[2 + i]*x[3 + i]*x[5 + i] +
        x[1 + i]*(x[3 + i]*x[5 + i] +
           x[2 + i]*(x[3 + i] + x[5 + i])) -
        2*x[i]*(x[3 + i]*x[5 + i] +
           x[2 + i]*(x[3 + i] + x[5 + i]) +
           x[1 + i]*(x[2 + i] + x[3 + i] + x[5 + i])) +
        3*(x[1 + i] + x[2 + i] + x[3 + i] + x[5 + i])*
         pow(x[i],2) - 4*pow(x[i],3))*pow(x[1 + i] - x[4 + i],-1)*
      pow(-x[i] + x[4 + i],-1)*pow(-x[2 + i] + x[4 + i],-1)*
      pow(-x[3 + i] + x[4 + i],-1)*pow(x[4 + i] - x[5 + i],-1) -
     u[5 + i]*(-(x[2 + i]*x[3 + i]*x[4 + i]) -
        x[1 + i]*(x[3 + i]*x[4 + i] +
           x[2 + i]*(x[3 + i] + x[4 + i])) +
        2*x[i]*(x[3 + i]*x[4 + i] +
           x[2 + i]*(x[3 + i] + x[4 + i]) +
           x[1 + i]*(x[2 + i] + x[3 + i] + x[4 + i])) -
        3*(x[1 + i] + x[2 + i] + x[3 + i] + x[4 + i])*
         pow(x[i],2) + 4*pow(x[i],3))*pow(x[1 + i] - x[5 + i],-1)*
      pow(-x[i] + x[5 + i],-1)*pow(-x[2 + i] + x[5 + i],-1)*
	      pow(-x[3 + i] + x[5 + i],-1)*pow(-x[4 + i] + x[5 + i],-1));
  else if (i==1)
    return  2*(u[-1 + i]*pow(x[-1 + i] - x[i],-1)*(x[2 + i]*x[3 + i] + x[1 + i]*(x[2 + i] + x[3 + i]) -
        2*x[i]*(x[1 + i] + x[2 + i] + x[3 + i]) + 3*pow(x[i],2))*pow(x[-1 + i] - x[1 + i],-1)*
      pow(x[-1 + i] - x[2 + i],-1)*pow(x[-1 + i] - x[3 + i],-1) +
     u[i]*pow(x[-1 + i] - x[i],-1)*(-(x[1 + i]*x[2 + i]) +
        x[-1 + i]*(3*x[i] - x[1 + i] - x[2 + i] - x[3 + i]) - x[1 + i]*x[3 + i] - x[2 + i]*x[3 + i] +
        3*x[i]*(x[1 + i] + x[2 + i] + x[3 + i]) - 6*pow(x[i],2))*pow(x[i] - x[1 + i],-1)*
      pow(x[i] - x[2 + i],-1)*pow(x[i] - x[3 + i],-1) +
     u[1 + i]*(x[2 + i]*x[3 + i] - 2*x[i]*(x[2 + i] + x[3 + i]) +
        x[-1 + i]*(-2*x[i] + x[2 + i] + x[3 + i]) + 3*pow(x[i],2))*pow(x[-1 + i] - x[1 + i],-1)*
      pow(x[i] - x[1 + i],-1)*pow(x[1 + i] - x[2 + i],-1)*pow(x[1 + i] - x[3 + i],-1) -
     u[2 + i]*(x[1 + i]*x[3 + i] - 2*x[i]*(x[1 + i] + x[3 + i]) +
        x[-1 + i]*(-2*x[i] + x[1 + i] + x[3 + i]) + 3*pow(x[i],2))*pow(x[i] - x[2 + i],-1)*
      pow(-x[-1 + i] + x[2 + i],-1)*pow(-x[1 + i] + x[2 + i],-1)*pow(x[2 + i] - x[3 + i],-1) -
     u[3 + i]*(x[1 + i]*x[2 + i] - 2*x[i]*(x[1 + i] + x[2 + i]) +
        x[-1 + i]*(-2*x[i] + x[1 + i] + x[2 + i]) + 3*pow(x[i],2))*pow(x[1 + i] - x[3 + i],-1)*
	       pow(-x[-1 + i] + x[3 + i],-1)*pow(-x[i] + x[3 + i],-1)*pow(-x[2 + i] + x[3 + i],-1));
  else if (i==N-2)
    return 2*u[-3 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i]) +
      x[-2 + i]*(x[-1 + i] - 2*x[i] + x[1 + i]))*pow(x[-3 + i] - x[-2 + i],-1)*
    pow(x[-3 + i] - x[-1 + i],-1)*pow(x[-3 + i] - x[i],-1)*pow(x[-3 + i] - x[1 + i],-1) -
   2*u[-2 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-1 + i]*(-2*x[i] + x[1 + i]) +
      x[-3 + i]*(x[-1 + i] - 2*x[i] + x[1 + i]))*pow(x[-3 + i] - x[-2 + i],-1)*
    pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-2 + i] - x[i],-1)*pow(x[-2 + i] - x[1 + i],-1) +
   2*u[-1 + i]*(x[i]*(3*x[i] - 2*x[1 + i]) + x[-2 + i]*(-2*x[i] + x[1 + i]) +
      x[-3 + i]*(x[-2 + i] - 2*x[i] + x[1 + i]))*pow(x[-3 + i] - x[-1 + i],-1)*
    pow(x[-2 + i] - x[-1 + i],-1)*pow(x[-1 + i] - x[i],-1)*pow(x[-1 + i] - x[1 + i],-1) -
   2*u[i]*pow(x[-2 + i] - x[i],-1)*(-3*x[-1 + i]*x[i] + x[-1 + i]*x[1 + i] - 3*x[i]*x[1 + i] +
      x[-2 + i]*(x[-1 + i] - 3*x[i] + x[1 + i]) +
      x[-3 + i]*(x[-2 + i] + x[-1 + i] - 3*x[i] + x[1 + i]) + 6*pow(x[i],2))*pow(-x[-3 + i] + x[i],-1)*
    pow(-x[-1 + i] + x[i],-1)*pow(x[i] - x[1 + i],-1) -
   2*u[1 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) + x[-3 + i]*(x[-2 + i] + x[-1 + i] - 2*x[i]) +
      x[i]*(-2*x[-1 + i] + 3*x[i]))*pow(x[-2 + i] - x[1 + i],-1)*pow(-x[-3 + i] + x[1 + i],-1)*
      pow(-x[-1 + i] + x[1 + i],-1)*pow(-x[i] + x[1 + i],-1);

  else if (i==N-1)
    return -2*u[-5 + i]*(x[i]*((3*x[-1 + i] - 4*x[i])*x[i] +
         x[-2 + i]*(-2*x[-1 + i] + 3*x[i])) +
      x[-3 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) +
         x[i]*(-2*x[-1 + i] + 3*x[i])) +
      x[-4 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) +
         x[-3 + i]*(x[-2 + i] + x[-1 + i] - 2*x[i]) +
         x[i]*(-2*x[-1 + i] + 3*x[i])))*
    pow(x[-5 + i] - x[-4 + i],-1)*pow(x[-5 + i] - x[-3 + i],-1)*
    pow(x[-5 + i] - x[-2 + i],-1)*pow(x[-5 + i] - x[-1 + i],-1)*
      pow(x[-5 + i] - x[i],-1) +
      2*u[-4 + i]*(x[i]*((3*x[-1 + i] - 4*x[i])*x[i] +
			 x[-2 + i]*(-2*x[-1 + i] + 3*x[i])) +
		   x[-3 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) +
			      x[i]*(-2*x[-1 + i] + 3*x[i])) +
		   x[-5 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) +
			      x[-3 + i]*(x[-2 + i] + x[-1 + i] - 2*x[i]) +
			      x[i]*(-2*x[-1 + i] + 3*x[i])))*
      pow(x[-5 + i] - x[-4 + i],-1)*pow(x[-4 + i] - x[-3 + i],-1)*
      pow(x[-4 + i] - x[-2 + i],-1)*pow(x[-4 + i] - x[-1 + i],-1)*
      pow(x[-4 + i] - x[i],-1) -
      2*u[-3 + i]*(x[i]*((3*x[-1 + i] - 4*x[i])*x[i] +
			 x[-2 + i]*(-2*x[-1 + i] + 3*x[i])) +
		   x[-4 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) +
			      x[i]*(-2*x[-1 + i] + 3*x[i])) +
		   x[-5 + i]*(x[-2 + i]*(x[-1 + i] - 2*x[i]) +
			      x[-4 + i]*(x[-2 + i] + x[-1 + i] - 2*x[i]) +
			      x[i]*(-2*x[-1 + i] + 3*x[i])))*
      pow(x[-5 + i] - x[-3 + i],-1)*pow(x[-4 + i] - x[-3 + i],-1)*
      pow(x[-3 + i] - x[-2 + i],-1)*pow(x[-3 + i] - x[-1 + i],-1)*
      pow(x[-3 + i] - x[i],-1) +
      2*u[-2 + i]*(x[i]*((3*x[-1 + i] - 4*x[i])*x[i] +
			 x[-3 + i]*(-2*x[-1 + i] + 3*x[i])) +
		   x[-4 + i]*(x[-3 + i]*(x[-1 + i] - 2*x[i]) +
			      x[i]*(-2*x[-1 + i] + 3*x[i])) +
		   x[-5 + i]*(x[-3 + i]*(x[-1 + i] - 2*x[i]) +
			      x[-4 + i]*(x[-3 + i] + x[-1 + i] - 2*x[i]) +
			      x[i]*(-2*x[-1 + i] + 3*x[i])))*
      pow(x[-4 + i] - x[-2 + i],-1)*pow(-x[-5 + i] + x[-2 + i],-1)*
      pow(-x[-3 + i] + x[-2 + i],-1)*pow(x[-2 + i] - x[-1 + i],-1)*
      pow(x[-2 + i] - x[i],-1) +
      2*u[-1 + i]*(x[i]*((3*x[-2 + i] - 4*x[i])*x[i] +
			 x[-3 + i]*(-2*x[-2 + i] + 3*x[i])) +
		   x[-4 + i]*(x[-3 + i]*(x[-2 + i] - 2*x[i]) +
			      x[i]*(-2*x[-2 + i] + 3*x[i])) +
		   x[-5 + i]*(x[-3 + i]*(x[-2 + i] - 2*x[i]) +
			      x[-4 + i]*(x[-3 + i] + x[-2 + i] - 2*x[i]) +
			      x[i]*(-2*x[-2 + i] + 3*x[i])))*
      pow(x[-4 + i] - x[-1 + i],-1)*pow(-x[-5 + i] + x[-1 + i],-1)*
      pow(-x[-3 + i] + x[-1 + i],-1)*pow(-x[-2 + i] + x[-1 + i],-1)*
      pow(x[-1 + i] - x[i],-1) +
      2*u[i]*pow(x[-4 + i] - x[i],-1)*
      (x[-3 + i]*x[-2 + i]*x[-1 + i] - 3*x[-3 + i]*x[-2 + i]*x[i] -
       3*x[-3 + i]*x[-1 + i]*x[i] - 3*x[-2 + i]*x[-1 + i]*x[i] +
       x[-4 + i]*(x[-2 + i]*(x[-1 + i] - 3*x[i]) +
		  x[-3 + i]*(x[-2 + i] + x[-1 + i] - 3*x[i]) -
		  3*(x[-1 + i] - 2*x[i])*x[i]) + 6*x[-3 + i]*pow(x[i],2) +
       6*x[-2 + i]*pow(x[i],2) + 6*x[-1 + i]*pow(x[i],2) +
       x[-5 + i]*(x[-2 + i]*x[-1 + i] +
		  x[-3 + i]*(x[-2 + i] + x[-1 + i] - 3*x[i]) +
		  x[-4 + i]*(x[-3 + i] + x[-2 + i] + x[-1 + i] - 3*x[i]) -
		  3*x[-2 + i]*x[i] - 3*x[-1 + i]*x[i] + 6*pow(x[i],2)) -
       10*pow(x[i],3))*pow(-x[-5 + i] + x[i],-1)*
      pow(-x[-3 + i] + x[i],-1)*pow(-x[-2 + i] + x[i],-1)*
      pow(-x[-1 + i] + x[i],-1);
  else return 0.;
}
#endif
