/** 
  * \class Reader reader.h "reader.h"
  *
  * \b Purpose:
  *  =============
  \verbatim 
  /********************************************************************************\
  /  Class for reading crystal parameters from a file                              \
  /                                                                                \
  /********************************************************************************\
  \endverbatim
  *
  * \author Joseph M. Gonzalez
  *
  * \version 0.1
  *
  * \date Sep 27, 2015 23:16:20
  *
  * \b Contact \n    jmgonza6@mail.usf.edu
  *
  */

#ifndef Reader_H
#define Reader_H

#include "common.h"

class Reader {
	public:
		double        alat;         //!< latttice parameter a (Å)
		double        blat;         //!< lattice parameter b (Å)
		double        clat;         //!< lattice parameter c (Å)
		double        alpha;        //!< angle beteween c & b
		double        beta;         //!< angle between c & a
		double        gamma;        //!< angle between a & b
		double        cta;          //!< c/a ratio, for use with Hexagonal(), h-BN(), Graphene()

		int           lbasis;       //!< number of basis atoms for graphene
		int           nx;           //!< supercell dimension along x
		int           ny;           //!< supercell dimension along y
		int           nz;           //!< supercell dimension along z
		int           fractional;   //!< fractional coordinates, 1 = true, 0 = false
		int           graphene;     //!< graphene structure was built, 1 = true, 0 = false
		int           custom;       //!< a custom structure was built, 1 = true, 0 = false

		int           resets;       //!< reset count
		int           run_count;    //!< run count, for debugging
		int           quit;         //!< quit signal
		int           view;         //!< render+build signal
		
		char          *pfile;       //!< parameter file to define the crystal
		char          *out_name;    //!< file name to write
		char          *file_format; //!< type of file to write
		
		char          *bravais;     //!< which lattice ?
		char          *lstyle;      //!< bravais lattice style, BCC
		char          *pd_struct;   //!< defines a library structure to use
		char          *element;     //!< which species ?

		std::vector<std::string>          elemList;    //!< define a custom stoichiometry (C,H,N,O)
		std::vector<int>                  elemCount;   //!< define a custom stoichiometry (2,4,1,3)
		std::vector<std::vector<double> > customBasis; //!< container for defining a custom basis set
		std::string                       customName;  //!< chemical formula of a custom structure

		Reader();
		~Reader();
		Parser* parser;

		void
		file(char *pf);

		int
		insane();
		
};
#endif