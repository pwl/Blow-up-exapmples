/**
 * @file   universal.c
 * @author Pawel Biernat <pawel.biernat@gmail.com>
 * @date   Mon Apr 13 12:44:46 2009
 * 
 * @brief this file contains functions of general use, utilized by
 *        various basis types. if a function you would like to write
 *        is usable by some other basis, include it here, this files
 *        header is included in other basis type headers
 * 
 * 
 */
#include "universal.h"


/* LINEAR TRANSFORMATION OF VARIABLES*/

H_DOUBLE h_y_of_x( void * basis, H_DOUBLE x )
{
  h_basis_params * p = ((h_basis*)(basis))->params;
  H_DOUBLE x0=p->x0, y0=p->y0;
  H_DOUBLE LX=p->LX, LY=p->LY;  
  
  return (x-x0)/LX*LY+y0;
}

H_DOUBLE h_x_of_y( void * basis, H_DOUBLE y )
{
  h_basis_params * p = ((h_basis*)(basis))->params;
  H_DOUBLE x0=p->x0, y0=p->y0;
  H_DOUBLE LX=p->LX, LY=p->LY;  

  return (y-y0)/LY*LX+x0;
}

H_DOUBLE h_dy_over_dx( void * basis )
{
  h_basis_params * p = ((h_basis*)(basis))->params;
  H_DOUBLE LX=p->LX, LY=p->LY;
  
  return LY/LX;
}

H_DOUBLE h_dx_over_dy( void * basis )
{
  h_basis_params * p = ((h_basis*)(basis))->params;
  H_DOUBLE LX=p->LX, LY=p->LY;
  
  return LX/LY;
}

/* END OF LINEAR TRANSFORMATION OF VARIABLES*/

/* DECOMPOSE FUNCTIONS */

/* int h_standard_decompose( void * basis, H_DOUBLE * fi, H_DOUBLE * ai ) */
/* { */
/*   h_basis * b = (h_basis*)basis; */
/*   H_DOUBLE * wi = b->weights; */
/*   H_DOUBLE * xi = b->collocation_points; */
/*   H_DOUBLE (*phi_n)(void*,int,H_DOUBLE) = b->functions->basis_nth; */
/*   H_DOUBLE (*y)(void*,H_DOUBLE) = b->functions->y_of_x; */
/*   H_DOUBLE a; */
/*   int N0 = b->params->N0; */
/*   int N1 = b->params->N1; */
/*   int i,j; */
  
/*   for ( i = N0-1; i <= N1+1; i++ ) */
/*     { */
/*       a=0.; */
      
/*       for ( j = N0; j <= N1; j++ ) */
/* 	{ */
/* 	  /\* rectangle integral formula *\/ */
/* 	  a+=wi[i]*fi[j]*phi_n(b,i,y(b,xi[j])); */
/* 	} */
/*       ai[i]=a; */
/*     } */
/*   return H_SUCCESS; */
/* } */

/* END OF DECOMPOSE FUNCTIONS */


/* COMPOSE FUNCTIONS */

/* int h_standard_compose( void * basis, H_DOUBLE * ai, H_DOUBLE * fi ) */
/* { */
/*   h_basis * b = (h_basis*)basis; */
/*   H_DOUBLE * xi = b->collocation_points; */
/*   H_DOUBLE (*phi_n)(void*,int,H_DOUBLE) = b->functions->basis_nth; */
/*   H_DOUBLE (*y)(void*,H_DOUBLE) = b->functions->y_of_x; */
/*   H_DOUBLE f; */
/*   int N0 = b->params->N0; */
/*   int N1 = b->params->N1; */
/*   int i,j; */
  
/*   for ( i = N0; i <= N1; i++ ) */
/*     { */
/*       f=0.; */

/*       for ( j = N0; j <= N1; j++ ) */
/* 	{ */
/* 	  f+=ai[j]*phi_n(b,i,y(b,xi[j])); */
/* 	} */
/*       fi[i]=f; */
/*     } */
/*   return H_SUCCESS; */
/* } */

/* END OF COMPOSE FUNCTIONS */
