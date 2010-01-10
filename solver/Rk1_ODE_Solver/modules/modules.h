#ifndef _MODULES_H_
#define _MODULES_H_

#include "includes.h"

/* general module structure */
typedef struct
{
  void (*init)(void * ODE_solver, void * ODE_module);
  void (*step)(void * ODE_solver, void * ODE_module);
  void (*stop)(void * ODE_solver, void * ODE_module);
  void (*data_free)(void * ODE_solver, void * ODE_module);
  H_DOUBLE dt;	   /* module is called every dt[e.g. every t_n=n*dt */
  H_DOUBLE t_last; /* time module was last executed */
  char name[100];  /* name of the module TODO: decrease table size */
  void * data;	   /* data specific to a particular module */
} ODE_module;

#endif /* _MODULES_H_ */
