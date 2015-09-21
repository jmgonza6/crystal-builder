/** 
  * \class Monoclinic monoclinic.h "monoclinic.h"
  *
  * \b Purpose:
  *  =============
  \verbatim 
  /******************************************************************************\
  /  Class to generate the basis atom positions, lattice vectors and lattice     \
  /  parameters, for the Monoclinic Bravais lattice class.                     \
  /  a != b != c, α = γ = 90 != β.                                                \
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


#ifndef MONOCLINIC_H
#define MONOCLINIC_H

#include "../util/common.h"
#include "lattice.h"

// 'P == primitive' 'I == body-centered' 'C == side-centered' 'F == face-centered'

class Monoclinic : public LATTICE {
	public:

		/** \brief Constructor for the Monoclinic class when accessed from a gui session or when reading a parameters file
		  * \param[in] i_alat - lattice parameter for `a`, in Å
		  * \param[in] i_blat - lattice parameter for `b`, in Å
		  * \param[in] i_clat - lattice parameter for `c`, in Å
		  * \param[in] i_lstyle - which cubic crystal style, \arg `SC`, `BCC`, `FCC`
          * \param[in] i_element - which element to assign to the crystal
          */
		// a(Å) b (Å) c(Å) 'BCC' 'FCC' 'SC'
		Monoclinic(double i_alat, 
			       double i_blat, 
			       double i_clat,
                   double i_beta,
                   int    i_apcell,
                   int    i_ntypes, 
			       char   i_lstyle[], 
			       char*  i_element);
		~Monoclinic();

	private:

		/** \brief Defines lattice vectors, lattice parameters, and basis atoms for Simple Cubic
          */
		void 
		primitive();

        /** \breif LiN3
        */
        void
        lin3();

		/** \brief Defines lattice vectors, lattice parameters, and basis atoms for Body Centered Cubic
          */
		void 
		BCC();

		/** \brief Defines lattice vectors, lattice parameters, and basis atoms for Face Centered Cubic
          */
		void 
		FCC();
};
#endif
