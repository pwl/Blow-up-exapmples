#ifndef _ODE_MODULES_H_
#define _ODE_MODULES_H_

#include "ODE_structs.h"

#define MOD_LIST_LENGTH 20


ODE_modules_list * ODE_modules_list_malloc ( ODE_solver * s );

void ODE_modules_list_init ( ODE_solver * s );

void ODE_modules_list_free ( ODE_solver * s );

void ODE_modules_add ( ODE_solver * s, ODE_module * m );

void ODE_modules_execute_init ( ODE_solver * s );

void ODE_modules_execute_step ( ODE_solver * s );

void ODE_modules_execute_stop ( ODE_solver * s );

ODE_module * ODE_modules_get_by_name ( ODE_solver * s, char * name );


#endif /*_ODE_MODULES_H_*/
