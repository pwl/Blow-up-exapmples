#ifndef _DERIVATIVES_H_
#define _DERIVATIVES_H_

#include "math.h"

/* #define D1_5pt_6pt */
/* #define D2_5pt_6pt */

/* #define D1_5pt */
/* #define D2_5pt */

#define D1_3pt
#define D2_3pt

double D1 ( double * u, double * x, int i, int N );
double D2 ( double * u, double * x, int i, int N );

#endif /* _DERIVATIVES_H_ */
