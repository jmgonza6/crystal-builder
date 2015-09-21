/* File		:	mathns.h */
/**
  *  \namespace MATHNS
  *  \brief Customized math operations and interfaces
  */

/** 
  * \namespace MATHNS
  *
  * \b Purpose:
  *  =============
  \verbatim 
  /*****************************************************************************\
  /  Customized math operations relevant for transforming and building crystals \
  /                                                                             \
  /*****************************************************************************\
  \endverbatim
  *
  * \author Joseph M. Gonzalez
  *
  * \version 0.2
  *
  * \date Sep 13, 2015 19:16:20
  *
  * \b Contact \n    jmgonza6@mail.usf.edu
  *
  */

#ifndef MATHNS_H
#define MATHNS_H
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <vector>
#include "common.h"


namespace MATHNS {
	// Level-1 operations

	/** \brief Compute magnitude of an n-dimensional vector \n
	  * \f$ \sqrt{\sum_{i}^{n} v_{i}*v_{i} } \f$
	  * \param v - Vector of data
	  * \returns Magnitude of \a v
	  */
	double 
	vector_mag(std::vector<double> v);

	/** \brief Compute dot product of two n-dimensional vectors \n
	  * \f$ \sum_{i}^{n} a_{i}*b_{i}  \f$
	  * \param a - Vector of data
	  * \param b - Vector of data
	  * \returns Dot product of \a a with \a b
	  */
	double 
	vector_dot(std::vector<double> a, 
		       std::vector<double> b
		       );

	/** \brief Compute angle, in radians, between two n-dimensional vectors \n
	  * \f$ \arccos \frac{\textbf{a} \cdot \textbf{b}}{|\textbf{a}||\textbf{b}|} \f$
	  * \param a - Vector of data
	  * \param b - Vector of data
	  * \returns Angle between \a a and \a b in radians
	  */
	double 
	angle(std::vector<double> a, 
		  std::vector<double> b
		  );

	// Level-2 operations
	/** \brief Compute cross product of two 3-dimensional vectors \n
	  * \f$ \textbf{a} \times \textbf{b} = \textbf{c} \f$
	  * \param a - Vector of data
	  * \param b - Vector of data
	  * \returns STL vector of length 3
	  */
	std::vector<double> 
	vector_cross(std::vector<double> a, 
		         std::vector<double> b
		         );

	/** \brief Compute matrix-vector product, naive implementation \n
	  * \f$ \textbf{A}\textbf{X} = \textbf{B}  \f$
	  * \param[in] lda - Leading dimension of matrix A
	  * \param[in] A - Pointer to 2D array A
	  * \param[in] X - 1D STL vector, X.size() must equal lda
	  * \returns STL vector of length lda
	  */
	std::vector<double> 
	mat_vec_mult(int                 lda, 
		         double**            A, 
		         std::vector<double> X
		         );

	/** \brief Rotate a 3D `std::vector` about an axis \n
	  * \param[in] X - 1D vector to be rotated through an `angle` about `axis`
	  * \param[in] axis - axis of rotation for current transformation
	  * \param[in] angle - angle to rotate through, in `(˚)`
	  * \returns \b `std::vector` of new coordinates
	  */
	std::vector<double> 
	rotate_vector(std::vector<double> X, 
		          std::string         axis, 
		          double              angle
		          );

	// Level-3 operations
}
#endif