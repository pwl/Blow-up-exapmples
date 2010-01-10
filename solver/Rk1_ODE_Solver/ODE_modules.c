#include "ODE_modules.h"



ODE_modules_list * ODE_modules_list_malloc ( ODE_solver * s )
{
  ODE_modules_list * modules =
    malloc ( sizeof ( ODE_modules_list ) );
  modules->mod_list =
    malloc ( MOD_LIST_LENGTH * sizeof ( ODE_module * ) );
  
  return modules;
}

void ODE_modules_list_init ( ODE_solver * s )
{
  s->modules = ODE_modules_list_malloc ( s );
  s->modules->n = 0;
}

void ODE_modules_list_free ( ODE_solver * s )
{
  ODE_module ** mods = s->modules->mod_list;
  int i,n = s->modules->n;
  if ( n > 0 )
    for ( i = 0; i < n; i++ )
      {
	/* if any special instructions to free module data are given,
	   use them */
	if ( mods[i]->data_free )
	  mods[i]->data_free( (void*)s, (void*)(mods[i]) );
	free ( mods[i] );
      }
  free ( s->modules->mod_list );
  free ( s->modules );
}

void ODE_modules_add ( ODE_solver * s, ODE_module * m )
{
  ODE_modules_list * mods = s->modules;

  if ( mods->n >= MOD_LIST_LENGTH )
    {
      printf ( "Warning! Trying to add to many ODE_modules, increase MOD_LIST_LENGTH.\n" );
      return;
    }
  mods->mod_list[mods->n] = m;
  (mods->n)++;
}

void ODE_modules_execute_init ( ODE_solver * s )
{
  ODE_module ** mods = s->modules->mod_list;
  int i,n = s->modules->n;
  for ( i = 0; i < n; i++ )
    {
      /* if any special instructions to free module data are given,
	 use them */
      mods[i]->init( (void*)s, (void*)mods[i] );
    }
}

void ODE_modules_execute_step ( ODE_solver * s )
{
  ODE_module ** mods = s->modules->mod_list;
  int i,n = s->modules->n;
  for ( i = 0; i < n; i++ )
    {
      /* if any special instructions to free module data are given,
	 use them */
      if ( mods[i]->dt == 0. )
	mods[i]->step( (void*)s, (void*)mods[i] );
      else if ( mods[i]->dt < 0. )
	{}
      else if (mods[i]->t_last + mods[i]->dt < *(s->state->t) )
	{
	  mods[i]->step( (void*)s, (void*)mods[i] );
	  mods[i]->t_last+=mods[i]->dt;
	}
    }
}

void ODE_modules_execute_stop ( ODE_solver * s )
{
  ODE_module ** mods = s->modules->mod_list;
  int i,n = s->modules->n;
  for ( i = 0; i < n; i++ )
    {
      /* if any special instructions to free module data are given,
	 use them */
      mods[i]->stop( (void*)s, (void*)mods[i] );
    }
}

ODE_module * ODE_modules_get_by_name ( ODE_solver * s, char * name )
{
  ODE_module ** mods = s->modules->mod_list;
  int i,n = s->modules->n;

  for ( i = 0; i < n; i++ )
    {
      if ( !( strcmp( name, mods[i]->name ) ) )
	return mods[i];
    }
  
  return NULL;
}
