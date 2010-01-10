/**
 * @file   structs.h
 * @author Pawel Biernat <pawel.biernat@gmail.com>
 * @date   Sun Apr 12 20:08:52 2009
 * 
 * @brief This header file contains all the structures used by basis
 * functions
 * 
 * 
 */
#ifndef _SPECTRAL_STRUCTS_H_
#define _SPECTRAL_STRUCTS_H_

#include "includes.h"

typedef struct {
  int N;			/**< Number of collocation points to
				   be used */
  H_DOUBLE x0, x1;		/* the range of the x */
  H_DOUBLE LX;			/* the length of the interval [x0,x1]:
				   x1-x0, has to be > 0 */
  H_DOUBLE y0, y1;		/* the range of the x' in the interval
				   specific to a given basis type x'
				   is from [a0,a1] */
  H_DOUBLE LY;			/* the length of the x' interval (a1-a0) */
  
} h_basis_params;



typedef struct {
  /* TODO: change the return types of all the int functions to return
     voids */

  /** compose and decompose function in this particular basis - they
      lay in the core of the derivative calculation when using
      spectral methods*/
  int (*compose) ( void * basis, H_DOUBLE * ai, H_DOUBLE * fi );
  int (*decompose) ( void * basis, H_DOUBLE * fi, H_DOUBLE * ai );
  
  /** calculate the derivative at collocation points. methods below are
      suggested to use efficient algorithms such as FFT or MMT with
      simmetrization due to their frequent usage inside time marching
      loop */
  int (*df_dx) ( void * basis, H_DOUBLE * fi, H_DOUBLE * D1fi );
  int (*d2f_dx2) ( void * basis, H_DOUBLE * fi, H_DOUBLE * D2fi );
  
  /** calculates two derivatives at once (optial if both derivatives
      are needed) */
  /* TODO: name needs changing */
  int (*derivatives) ( void * basis, H_DOUBLE * fi, H_DOUBLE * dfi, H_DOUBLE * d2fi );

  /** function used to generate the collocation points */
  int (*col_pts_gen) ( void * basis, H_DOUBLE * xi );
  
  /** the reason of writting the function below is to make the
      programming of new basis sets easier and more transparent */
  /** the linear transformation from [x0,x1] interval into the interval
      specific for a given basis: [y0,y1], its inverse and their
      derivatives */
  H_DOUBLE (*y_of_x) (void * basis, H_DOUBLE x);
  H_DOUBLE (*x_of_y) (void * basis, H_DOUBLE y);

  /** if the x_to_a should be y=y(x), the functions below correspond to
      the y'(x) and x'(y), and so they should be placed before the
      (d/dy)(f(y(x))) operator if we are willing to get a derivative in
      the x variable and vice versa due to the chain rule */
  H_DOUBLE (*dx_dy) (void * basis);
  H_DOUBLE (*dy_dx) (void * basis);
  
  /** weights generator, calculates and saves the weights at
      collocation points into the table wi */
  int (*weights_gen) (void * basis, H_DOUBLE * wi);
  
  /** Given number of basis elements to consider, interval and basis
      functions this function returns the basis parameters */
  h_basis_params * (*params_init) ( int N, H_DOUBLE x0, H_DOUBLE x1,
				    void * functions );

  /** function used to initialize/free additional basis-specific
      variables/tebles/FFTW plans etc. */
  void (*additional_init) (void * basis);
  void (*additional_free) (void * basis);
  
} h_basis_functions;


/* the structure collecting the basis and its parameters along with
    variables generated during the init stage */
typedef struct
{
  /* to be included: */
  /*   int basis_ID; */
    /* char * name; */
  
  h_basis_functions * functions; /**< the basis functions*/
  h_basis_params * params;	 /**< the parametes */
  
  /* variables generated at initialization */
  /** collocation points table, ranges from 0 to N=N1-N0+1, but uses a
      bit more (N+1) space just in case */
  H_DOUBLE * collocation_points;
  /** quadrature weights table, ranges from 0 to N=N1-N0+1+1 (same as
      above) */
  H_DOUBLE * weights;
  
  /** few temporary structures used to avoid multiple mallocing and
      freeing system memory */
  H_DOUBLE * ai_temp;
  H_DOUBLE * fi_temp;

  H_DOUBLE * dfi_temp;
  H_DOUBLE * d2fi_temp;
  
  /** pointer to basis specific structures initialized during the
     type->additional_init(b) */
  void * basis_specific;
  
} h_basis;

#endif
