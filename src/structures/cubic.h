/** 
  * \class Cubic cubic.h "cubic.h"
  *
  * \b Purpose:
  *  =============
  \verbatim 
  /******************************************************************************\
  /  Class to generate the basis atom positions, lattice vectors and lattice     \
  /  parameter a, for the Cubic Bravais lattice class.                           \
  /  a = b = c, α = β = γ = 90.                                                  \
  /  Within this class, there are 3 available cubic styles that can be generated \
  /  Simple Cubic, Body Centered, Face Centered, and Diamond                     \
  /  * * Inherits from the LATTICE class                                         \
  /                                                                              \ 
  /******************************************************************************\
  \endverbatim
  *
  * \author Joseph M. Gonzalez
  *
  * \version 0.1
  *
  * \date Sep 13, 2015 19:16:20
  *
  * \b Contact \n    jmgonza6@mail.usf.edu
  *
  */

#ifndef CUBIC_H
#define CUBIC_H

#include "../util/common.h"
#include "lattice.h"

// 'P == primitive' 'I == body-centered' 'C == side-centered' 'F == face-centered'

class Cubic : public LATTICE {

	public:

		/** \brief Constructor for the Cubic Bravais class when accessed from a gui session or when reading a parameters file
		  * \param[in] i_alat - lattice parameter for `a`, in Å
		  * \param[in] i_lstyle - which cubic crystal style, \arg `SC`, `BCC`, `FCC`, `Diamond`
          * \param[in] i_element - which element to assign to the crystal
          */
		Cubic(double i_alat, 
			  char   i_lstyle[], 
			  char*  i_element);

		~Cubic();

	private:

		/** \brief Defines lattice vectors, lattice parameters, and basis atoms for Simple Cubic
          */
		void 
		primitive();

		/** \brief Defines lattice vectors, lattice parameters, and basis atoms for Body Centered Cubic
          */
		void 
		BCC();

		/** \brief Defines lattice vectors, lattice parameters, and basis atoms for Face Centered Cubic
          */
		void 
		FCC();

		/** \brief Defines lattice vectors, lattice parameters, and basis atoms for Diamond
          */
		void 
		Diamond();
		
};
#endif
