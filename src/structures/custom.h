/** 
  * \class Custom custom.h "custom.h"
  *
  * \b Purpose:
  *  =============
  \verbatim 
  /***************************************************************************************\
  /  Class to create a custom lattice from scratch by specifying the lattice              \
  /  parameters, (a, b, c) and the lattice angles (α, β, γ )                              \
  /  Additionally, the basis can be defined on the fly with any number of                 \
  /  atomic species within the unit cell.  The basis coordinates are assumed              \
  /  to be in fractional units, and in the correct stoichiometric order,                  \
  /  i.e. PETN = C10 H16 8N 24O ==> 1-11 basis atoms to C, 12-28 basis atoms to H, etc..  \
  /                                                                                       \ 
  /  The orientation of the crystal is such that a1 is colinear with the x-axis.          \
  /  a2 is obtained by rotating a1 by γ about the z-axis                                  \
  /  a3 is obtained by rotating a1 by -β about the y-axis                                 \
  /***************************************************************************************\
  \endverbatim
  *
  * \author Joseph M. Gonzalez
  *
  * \version 0.1
  *
  * \date Sep 20, 2015 15:16:20
  *
  * \b Contact \n    jmgonza6@mail.usf.edu
  *
  */


#ifndef Custom_H
#define Custom_H

#include "../util/common.h"
#include "lattice.h"


class Custom : public LATTICE {
	public:


		/** \brief Constructor for creating a custom crystal when accessed from a gui session or when reading a parameters file
          * \param[in] latPars - array containing the lattice parameters in (Å)
          *  \arg latPars[0] = `a`, \arg latPars[1] = `b`, \arg latPars[2] = `c`
          * \param[in] latAngles - array containing the lattice angles in (˚)  
          *  \arg latAngles[0] = `α`, \arg latAngles[1] = `β`, \arg latAngles[2] = `γ`
          * \param[in] positions - basis atom positions
          * \param[in] customName - name given to the custom lattice, currently this is the formula unit, i.e. MoS2
          * \param[in] elemList - list of the atomic species specified by the user
          * \param[in] elemCount - list containing the stoichiometric coefficients for elements in `elemList`
          */
        Custom(double                              latPars[3],
               double                              latAngles[3],
               std::vector<std::vector<double> >   positions,
               std::string                         customName, 
               std::vector<std::string>            elemList,
               std::vector<int>                    elemCount);

        ~Custom();
};
#endif