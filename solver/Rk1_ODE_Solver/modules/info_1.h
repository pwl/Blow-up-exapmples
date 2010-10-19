#ifndef _INFO_1_H_
#define _INFO_1_H_

#include "../ODE_structs.h"


/* data specific to this module */
typedef struct
{
  /* FILE ** file; */
  /* FILE * file_1, * file_2, * file_3, * file_4; */
  /* int pt1, pt2, pt3, pt4; */
  char ** fileNames;
  int * pt;
  int Npt;
} info_1_module_data;

/* standard module functions */
void info_1_init ( void * solver, void * module );

void info_1_step ( void * solver, void * module );

void info_1_stop ( void * solver, void * module );

void info_1_data_free ( void * solver, void * module );

/* module init function, can be programmed to take any arguments,
   and is called by the user */
ODE_module * ODE_module_info_1_init (H_DOUBLE dt, int N );

double D1 ( double * u, double * x, int i, int N );
double D2 ( double * u, double * x, int i, int N );

double D3_at_0 (double * u, double * x);
double D1_at_0 (double * u, double * x);

#endif /* _INFO_1_H_ */
