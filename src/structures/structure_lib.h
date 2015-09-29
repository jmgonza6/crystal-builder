/** 
  * \class StructureLib structure_lib.h "structure_lib.h"
  *
  * \b Purpose:
  *  =============
  \verbatim 
  /******************************************************************************\
  /  Class to generate the basis atom positions, lattice vectors and lattice     \
  /  parameters for various 2D materials  and Energetic materials, specifically, \
  /  2D materials: Graphene, h-BN                                                \
  /                MDC-2H/1T i.e.  (Pb,Sn)(S,Se,Te)2                             \
  /                TMDC-2H/1T i.e. (Mo,W)(S,Se,Te)2                              \
  /  Energetic materials: PETN-I, TATB, β-HMX                                    \
  /                                                                              \
  /  * * Inherits from the LATTICE class                                         \
  /                                                                              \ 
  /******************************************************************************\
  \endverbatim
  *
  * \author Joseph M. Gonzalez
  *
  * \version 0.3
  *
  * \date Sep 13, 2015 19:16:20
  *
  * \b Contact \n    jmgonza6@mail.usf.edu
  *
  */

#ifndef STRUCTURE_LIB_H
#define STRUCTURE_LIB_H

#include "../util/common.h"
#include "lattice.h"


class StructureLib : public LATTICE {
	public:

		/** \brief Constructor for the graphene class when accessed from a gui session or when reading a parameters file
		  * \param[in] i_lstyle - which crystal to build \arg `h-BN` or `MDC 2H/1T` or `TMDC 2H/1T` or `PETN-I` or `TATB` or `β-HMX`
          * \param[in] basisAtoms - total number of basis atoms used for building graphene, \arg 2 or 4
          * \param[in] i_alat - `a` lattice parameter
          * \param[in] i_clat - `c` lattice parameter
          * \param[in] elements - list of element symbols
          */
		StructureLib(char                     i_lstyle[],  
                     int                      basisAtoms,
                     double                   i_alat,
                     double                   i_clat,
                     std::vector<std::string> elements);

		~StructureLib();

	private:
        double cta;

		// 2d layered materials
        /** \brief Constrct an hexagonal 2 atom unit cell or a 4 atom rectangular unit cell
           \verbatim
                  _________            _________________           ^                      
                 /        /           |                 |          |                      
                /    o   /            |    o     o      |          |                      
               /  o     /             |                 |          |                      
              /________/              |o____________o___|          |------------> x         
                  2                           4                                           
           \endverbatim
          * \param[in] i_alat - lattice parameter for `a`, Å
          * \param[in] i_clat - `c` lattice parameter
          * \param[in] basisAtoms - \arg \b `2` for hexagonal, \arg \b `4` rectangular
          */
        void
        graphene(double i_alat, 
                 double i_clat,
                 int    basisAtoms);

        /** \brief Define the basis atoms and lattice for hexagonal Boron Nitride
          * \brief `a` = `b` = 2.505 Å, `c` = 12 Å
          */
		void
		h_bn();

        /** \brief Define the basis atoms and lattice for a single layer of a metal dichalcogenide, \n 
          *  i.e. (Pb,Sn)(S,Se,Te)2 in the 2H polytype
          * \param[in] i_alat - `a` lattice parameter, assumed that `a` = `b`
          * \param[in] i_clat - `c` lattice parameter, 
          * \param[in] elements - list of element symbols
          */
		void 
		mdc_2h(double                    i_alat, 
               double                    i_clat,
                std::vector<std::string> elements);

        /** \brief Define the basis atoms and lattice for a single layer of a metal dichalcogenide, \n 
          *  i.e. (Pb,Sn)(S,Se,Te)2 in the 1T polytype
          * \param[in] i_alat - `a` lattice parameter, assumed that `a` = `b`
          * \param[in] i_clat - `c` lattice parameter
          * \param[in] elements - list of element symbols
          */
		void 
		mdc_1t(double                    i_alat,
                double                    i_clat,
                std::vector<std::string> elements);

        /** \brief Define the basis atoms and lattice for a single layer of a transition metal dichalcogenide, \n 
          *  i.e. (Mo,W)(S,Se,Te)2 in the 2H polytype
          * \param[in] i_alat - `a` lattice parameter, assumed that `a` = `b`
          * \param[in] i_clat - `c` lattice parameter
          * \param[in] elements - list of element symbols
          */
        void
        tmdc_2h(double                   i_alat, 
                double                    i_clat,
                std::vector<std::string> elements);

        /** \brief Define the basis atoms and lattice for a single layer of a transition metal dichalcogenide, \n 
          *  i.e. (Mo,W)(S,Se,Te)2 in the 1T polytype
          * \param[in] i_alat - `a` lattice parameter, assumed that `a` = `b`
          * \param[in] i_clat - `c` lattice parameter
          * \param[in] elements - list of element symbols
          */
        void
        tmdc_1t(double                   i_alat, 
                double                    i_clat,
                std::vector<std::string> elements);	
		
		// energetic materials
        /** \brief Define the basis atoms and lattice for a single crystal of PETN-I
          * \brief `a` = `b` = 9.38 Å, `c` = 6.71 Å
          */
		void 
		petn();

        /** \brief Define the basis atoms and lattice for a single crystal of TATB
          * \brief `a` = `b` = `c` = 9.01  Å
          */
		void 
		tatb();

        /** \brief Define the basis atoms and lattice for a single crystal of HMX
          */
		void
		hmx();

};
#endif
