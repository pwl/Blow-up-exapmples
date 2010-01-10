#include "finite_difference_5.h"

/* COLLOCATION POINTS GENERATORS */

int h_finite_difference_5_col_pts( void * basis,
		       H_DOUBLE * xi )
{
  h_basis * b = (h_basis*)(basis);
  H_DOUBLE N = (H_DOUBLE)(b->params->N);
  H_DOUBLE (*x)( void *, H_DOUBLE ) = b->functions->x_of_y;
  H_DOUBLE i;
  
  for ( i = 0.; i <= N-1.; i++ )
    xi[(int)i]=x(b,2.*i/(N-1)*PI);

  return H_SUCCESS;
}

/* END OF COLLOCATION POINTS GENERATORS */


/* WEIGHTS GENERATION */

int h_finite_difference_5_weights_gen( void * basis, H_DOUBLE * wi )
{
  h_basis * b = (h_basis*)basis;
  int i, N = b->params->N;
  H_DOUBLE w = 1./((H_DOUBLE)(N-1));
  
  for ( i = 0; i <= N-1; i++ )
    wi[i]=w;
  
  return H_SUCCESS;
}

/* END OF WEIGHTS GENERATION */

h_basis_params * h_finite_difference_5_params_init ( int N, H_DOUBLE x0, H_DOUBLE x1,
				      void * type )
{
  h_basis_params * p = malloc(sizeof(h_basis_params));
  p->N = N;
  p->x0 = x0;
  p->x1 = x1;
  p->LX = x1-x0;
  /* in this method the parameters below aren't necessary */
  p->y0=0.;
  p->y1=2*PI;
  p->LY = 2*PI;
  
  return p;
}

/* FINITE_DIFFERENCE_5 BASIS INITIALIZATION PROCEDURE */

/* all the h_basis members are explained in the structs.h */
h_basis_functions * h_basis_finite_difference_5_function_init ( void )
{
  /* initialization of finite_difference_5 basis */
  h_basis_functions * finite_difference_5_basis =
    (h_basis_functions*) malloc (sizeof (h_basis_functions) );

  finite_difference_5_basis->derivatives = 
    &h_basis_finite_difference_5_derivatives;
  
  finite_difference_5_basis->col_pts_gen = 
    &h_finite_difference_5_col_pts;
  
  finite_difference_5_basis->y_of_x =
    &h_y_of_x;
  
  finite_difference_5_basis->x_of_y = 
    &h_x_of_y;
  
  finite_difference_5_basis->dx_dy = 
    h_dx_over_dy;
  
  finite_difference_5_basis->dy_dx = 
    h_dy_over_dx;
  
  finite_difference_5_basis->weights_gen = 
    &h_finite_difference_5_weights_gen;
  
  finite_difference_5_basis->additional_init = 
    NULL;
  
  finite_difference_5_basis->additional_free = 
    NULL;
  
  finite_difference_5_basis->params_init = 
    &h_finite_difference_5_params_init;
  
  return finite_difference_5_basis;
  
}

/* END OF FINITE_DIFFERENCE_5 BASIS INITIALIZATION PROCEDURE */


/* FINITE_DIFFERENCE_5 BASIS FREE PROCEDURE */

void h_basis_finite_difference_5_free ( h_basis_functions * b )
{
  free( b );
}

/* END OF FINITE_DIFFERENCE_5 BASIS FREE PROCEDURE */


/* CALCULATE TWO DERIVATIVES AT ONCE */

int h_basis_finite_difference_5_derivatives ( void * basis, H_DOUBLE * fi, H_DOUBLE * dfi, H_DOUBLE * d2fi )
{
  h_basis * b = (h_basis*)(basis);
  int i,N = b->params->N;
  H_DOUBLE LX = b->params->LX;
  H_DOUBLE dx = LX/(N-1.);
  
  for ( i = 2; i < N-3; i++ )
    {
      dfi[i]= (-fi[i+2]+fi[i-2]+8*(fi[i+1]-fi[i-1]))/12./dx;
      d2fi[i]= (-fi[i+2]-fi[i-2]+16.*(fi[i+1]+fi[i-1])-30.*fi[i])/12./dx/dx;
    }

  dfi[0]=1./12./dx*(-25.*fi[0]+48.*fi[1]-36.*fi[2]+16.*fi[3]-3.*fi[4]);
  dfi[1]=(fi[2]-fi[0])/2./dx;

  dfi[N-2]=(fi[N-1]-fi[N-3])/2./dx;
  dfi[N-1]=(fi[N-1]-fi[N-2])/dx;

  d2fi[0]=0.;
  d2fi[1]=(fi[0]+fi[2]-2.*fi[1])/dx/dx;
  
  d2fi[N-2]=(fi[N-1]+fi[N-3]-2.*fi[N-2])/dx/dx;
  d2fi[N-1]=0.;

  return H_SUCCESS;
}

/* END OF CALCULATE TWO DERIVATIVES AT ONCE */
