#ifndef _ODE_STATE_H_
#define _ODE_STATE_H_

#include "ODE_structs.h"

ODE_state * ODE_state_malloc ( ODE_solver * s );

void ODE_state_init ( ODE_solver * s );

void ODE_state_free ( ODE_solver * s );



#endif /*_ODE_STATE_H_*/
