/** 
  * \class Tetragonal tetragonal.h "tetragonal.h"
  *
  * \b Purpose:
  *  =============
  \verbatim 
  /******************************************************************************\
  /  Class to generate the basis atom positions, lattice vectors and lattice     \
  /  parameters, for the Tetragonal Bravais lattice class.                       \
  /  a = b != c, α = β = γ = 90.                                                 \
  /  Within this class, there are 3 available cubic styles that can be generated \
  /  Simple Cubic, Body Centered, Face Centered                                  \
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

#ifndef TETRAGONAL_H
#define TETRAGONAL_H

#include "../util/common.h"
#include "lattice.h"

// 'P == primitive' 'I == body-centered' 'C == side-centered' 'F == face-centered'

class Tetragonal : public LATTICE {
	public:

		// a(Å) c(Å) 'BCC' 'FCC' 'SC'
		Tetragonal(double i_alat, double i_clat, char i_lstyle[], char *i_element);
		~Tetragonal();

	private:
		void primitive(); // SC
		void BCC();
		void FCC();
};
#endif
