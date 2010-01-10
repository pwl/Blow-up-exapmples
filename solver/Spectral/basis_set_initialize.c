#include "basis_set_initialize.h"

/** 
 * Function used to initialize basis given its type and basic
 * parameters.
 * 
 * @param N number of elements of created basis (also coresponds to
 * the number of collocation points created
 * @param x0 beginning of the interval on which basis is used
 * @param x1 end of the interval on which basis is used
 * @param type type of the basis (actualy a set of methods used to
 * initialize and as and interface to the basis
 * 
 * @return the pointer to the created basis
 */
h_basis * h_basis_initialize (int N, H_DOUBLE x0, H_DOUBLE x1,
			      h_basis_functions * type )
{
  h_basis * b;
  /*   h_basis_params * params; */
  printf ( "** Initializing basis for\n" );
  printf ( "   N = %i, x0 = %f, x1 = %f\n",
	   N, x0, x1 );
  
  /* allocate some memory for the new basis */
  b = malloc (sizeof(h_basis));
  
  /* setup params */
  b->params = type->params_init( N, x0, x1, type );
  
  /* allocate memory for tables */
  h_basis_tables_alloc ( b );
  
  /* set basis type */
  b->functions = type;
  
  /* initialize basic features of the basis */
  type->col_pts_gen( b, b->collocation_points );
  type->weights_gen( b, b->weights );
  
  /* initialize additional basis-specific inits */
  if ( type->additional_init )
    type->additional_init( b );

  printf ( "** Basis initialization done!\n" );
  
  return b;
}

/** 
 * This function is ment to be the part of API to the h_basis
 * structure. Note: after freeing the basis does not free the
 * h_basis_functions used to initialize it, so it has to be freed
 * separately
 * 
 * @param b basis to free
 */
void h_basis_free ( h_basis * b )
{
  printf( "** Freeing basis... " );
  if ( b->functions->additional_free )
    b->functions->additional_free( b );
  
  h_basis_tables_free(b);
  h_params_free(b->params);
  free( b->functions );
  
  free(b);
  printf( "Done!\n" );
}

/* doesn't fit here very much - its h_basis_params specific, so it
   should be moved somewhere around there */
void h_params_free ( h_basis_params * p )
{
  free ( p );
}


/* allocates some memory for the basis internal structure such as
   arrays and matrices */
void h_basis_tables_alloc ( h_basis * b )
{
  /* the N+1 length is due to some algorithms needing more than N
     space (it's false)*/
  int N = b->params->N;

  b->collocation_points = malloc( N * sizeof( H_DOUBLE ) );
  b->weights = malloc( N * sizeof( H_DOUBLE ) );
  b->ai_temp = malloc( N * sizeof( H_DOUBLE ) );
  b->fi_temp = malloc( N * sizeof( H_DOUBLE ) );
  b->dfi_temp = malloc( N * sizeof( H_DOUBLE ) );
  b->d2fi_temp = malloc( N * sizeof( H_DOUBLE ) );
}

/* frees the memory allocated for all the arrays */
void h_basis_tables_free ( h_basis * b )
{
  free(b->collocation_points);
  free(b->weights);
  free(b->ai_temp);
  free(b->fi_temp);
  free(b->dfi_temp);
  free(b->d2fi_temp);
}

