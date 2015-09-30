/** 
  * \class INTERFACE interface.h "interface.h"
  *
  * \b Purpose:
  *  =============
  \verbatim 
  /*****************************************************************************\
  /  Class which constructs all of the core routines needed to build a crystal. \
  /  Specifically, the Gui class, LATTICE class, and the Write class are all    \
  /  constructed when this class is constructed.  This class serves to access   \
  /  the data from Gui and send it to the LATTICE and Write classes.            \
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

#ifndef INTERFACE_H
#define INTERFACE_H

#include "../util/common.h"

#include "../structures/lattice.h"
#include "../structures/cubic.h"
#include "../structures/custom.h"
#include "../structures/hexagonal.h"
#include "../structures/monoclinic.h"
#include "../structures/orthorhombic.h"
#include "../structures/structure_lib.h"
#include "../structures/tetragonal.h"

#include "../util/write.h"
#include "../util/reader.h"
#include "../gui/gui.h"

class INTERFACE {
	
	private:
		double       i_alat;
		double       i_cta;


		int          lclass;
		int          lstyle;
		int          i_lorient;
		int          i_species;
		std::string  atom_list;

		char         *oformat;
		char         *format;
		char         *fname;
		char         *stylename;


		/** \brief Guide the user through an interactive command line menu to define the crystal parameters. \n
		  * Marked deprecated as of v 0.3!
		  */
		void 
		menu();

		/** \brief Display program info at the command line
		  */
		void 
		banner();

		/** \brief Read the atomic configuration from a text file
		  * \param[in] fptr - Filename of the configuration file
		  */
		void 
		input(char *fptr);

	public:
		std::vector<std::string> species;   //!< container to hold each element string defined by the user

		Gui*                      gui;      //!< pointer to the gui class
		LATTICE*                  lattice;  //!< pointer to the lattice class, each Bravais crystal inherits from LATTICE
		Write*                    write;    //!< pointer to the write class, used for writing specific formats
		Errors*                   errors;
		Parser*                   parser;
		Reader*                   reader;

		/** \brief Constructor to start up all core level classes
		  * \param[in] nargs - number of command line arguments
		  * \param[in] argv - char array containing all command line arguments
		  * \param[in] fptr - name of parameters file to read from
		  */
		INTERFACE(int        nargs, 
			      char       **argv, 
			      const char *fptr);

		/** \brief Destructor to free all memory allocated for the core classes
		  */
		~INTERFACE();

		/** \brief Display all the parameters entered by the user during the GUI session
		  */
		void 
		show_parameters();

		/** \brief Store all the parameters and build the required crystal. \n
		  * This is where each of the specific Bravais lattices are instantiated as an \n
		  * object of the LATTICE class.
		  */
		void 
		build();

		/** \brief Store all the parameters and build the required crystal. \n
		  * This is where each of the specific Bravais lattices are instantiated as an \n
		  * object of the LATTICE class.
		  */
		void 
		build_from_file();

		/** \brief Write the as built crystal to a regular file. \n
		  * This method accesses the requested file format and passes all the necessary data for that format.
		  */
		void 
		write_data(int src);

		/** \brief Simple sanity check to ensure all necessary parameters have been defined .\n
		  *  If a required parameter was not set, then a detailed message on the missing parameter\n
		  *  is printed to the terminal via \b `stderr`.
		  * \returns `0` if successful, `1` otherwise
		  */
		int 
		insane();
		
};


#endif

