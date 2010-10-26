#ifndef _MM_DISTRIBUTE_POINTS_H_
#define _MM_DISTRIBUTE_POINTS_H_

#include "solver/Rk1_ODE_Solver/ODE_solver.h"
#include "stdio.h"
#include "derivatives.h"

#define CYAN1		"\x1b[0;36m"
#define RED1		"\x1b[1;31m"
#define GREEN1		"\x1b[1;32m"
#define BOLD1		"\x1b[1m"
#define FORMAT_OFF	"\x1b[0m"

double mm_u(double x);
double mm_du(double x);
double mm_ddu(double x);
double mm_M(double x);
void mm_ODE_set(void *solver, double t, double *y, double *f);
void mm_setup_mesh(double *x, int N);

#endif /* _MM_DISTRIBUTE_POINTS_H_ */
