/** 
  * \class Gui gui.h "gui.h"
  *
  * \b Purpose:
  *  =============
  \verbatim 
  /*****************************************************************************\
  /  Class which initializes, displays, and monitors all activity in the GUI    \
  /                                                                             \
  /*****************************************************************************\
  \endverbatim
  *
  * \author Joseph M. Gonzalez
  *
  * \version 0.9
  *
  * \date Sep 13, 2015 19:16:20
  *
  * \b Contact \n    jmgonza6@mail.usf.edu
  *
  */

#ifndef GUI_H
#define GUI_H

#include "../util/common.h"

#define FRWIDTH  600                //!< width of the frame inside each tab
#define FRHEIGHT 400                //!< height of the frame inside each tab

class Gui {

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


		/** \brief Constructor launches an instances of the parser class to handle 
		  * the string maniupulations. It also defines all of the global variables 
		  * to a reasonable value so that the app can be run with as little interaction 
		  * as possible
		  */
		Gui();

		~Gui();

		// create a notebook style gui with 3 main pages
		// tabs located along the top, and 4 action buttons
		// located along the bottom

		/** \brief Initialize the GTK environment and define
		  *  layout of the gui. creates a notebook style gui with 4 main tabs
		  *  and 4 buttons for rendering, building, reseting, and exiting
		  * \param window - In which window do we create the gui
		  */
		void 
		display_in(GtkWidget *window);

		/** \brief Store the global variables set by the callback functions
		  *  layout of the gui
		  */
		void 
		save_settings();

	private:

		Parser *parser;


		// fill in a dropdown menu according to the style
		// 'atoms' -> load with atomic symbols 'formats' -> writing formats
		GtkWidget 
		*fill_dropdown(const char *style);

		// search for an input file using a file browser
		static 
		void 
		open_browser(GtkWidget* button, gpointer window);

		// save files using a file browser
		static 
		void 
		save_browser(GtkWidget* button, gpointer window);

		// build the defined crystal structure
		static 
		void 
		build_crystal(GtkWidget* button, gpointer window);


		// reset the gui parameters/switches for plotting
		// a new set of data
		static 
		void 
		reset_parameters(GtkWidget* button, gpointer window);


		// open pop-up window detailing any errors during the
		// read/write/plotting process
		static 
		void 
		open_dialog(const char *syst, char message[MAX_STRING_LENGTH]);

		// show a reminder dialog to define the lattice parameters 
		// and stoichiometry when building one of the (T)MDC crystals
		static
		void
		reminder();

		// open a dialog with the parameters unique to graphene
		static 
		void 
		graphene_dialog();

		// open a dialog which contains dropdown menus of the 
		// library structures found in the StructureLib class
		static 
		void 
		structures_dialog();


		/* Callback functions to set user inputs */

		//
		// General parameters
		//

		// set the user defined nx value based on entry being 'activated'
		static 
		void 
		set_nx(GtkWidget *entry, gpointer data);

		// set the user defined ny value based on entry being 'activated'
		static 
		void 
		set_ny(GtkWidget *entry, gpointer data);

		// set the user defined nz value based on entry being 'activated'
		static 
		void 
		set_nz(GtkWidget *entry, gpointer data);
		
		// set the user defined single atomic species by selecting from a dropdown
		static 
		void 
		set_atom_species( GtkComboBox *dropdown, gpointer data );
		
		// define the output format of the file, i.e. LAMMPS, VASP ...
		static
		void 
		set_write_style( GtkComboBox *dropdown, gpointer data );
		
		// set the user defined bravais lattice from a dropdown
		static 
		void 
		set_bravais( GtkComboBox *dropdown, gpointer data );
		
		// which class of a certain bravais lattice to build, i.e. Cubic->FCC, Cubic->BCC ...
		static 
		void 
		set_lattice_style(GtkComboBox *widget, gpointer data);

		// wrapper to gtk_main_quit()
		static 
		void 
		quit_main(GtkWidget *widget, gpointer data);

		// call back to use when the user wants to build and view
		// the crystal.  This calls gtk_main_quit() but it also
		// sets a public member view = 1.  gui->view is analyzed
		// in main.
		static 
		void 
		render_crystal(GtkWidget *widget, gpointer data);


		// used to actually store the data, called after the user closes
		// the gui session.  This is so the user can freely change parameters
		// or reset them all to start over
		void
		define_basis_atoms();

		
		//
		// Custom options
		//

		// choose a pre-defined crystal from a dropdown menu
		static 
		void 
		set_pd_struct( GtkComboBox *dropdown, gpointer data );

		// define a custom stoichiometry for the crystal which in turn
		// is used to define the number of atoms in the cell and define
		// the different element types to the proper basis
		static 
		void 
		set_custom_species(GtkWidget *entry, gpointer data);

		// set the user defined a lattice parameter based on entry being 'activated'
		static 
		void 
		set_alat(GtkWidget *entry, gpointer data);

		// set the user defined b lattice parameter based on entry being 'activated'
		static 
		void 
		set_blat(GtkWidget *entry, gpointer data);

		// set the user defined c lattice parameter based on entry being 'activated'
		static 
		void 
		set_clat(GtkWidget *entry, gpointer data);

		// set the user defined lattice angle alpha, between b&c
		// based on entry being 'activated'
		static 
		void 
		set_alpha(GtkWidget *entry, gpointer data);

		// set the user defined lattice angle beta, between a&c
		// based on entry being 'activated'
		static 
		void 
		set_beta(GtkWidget *entry, gpointer data);

		// set the user defined lattice angle gamma, between a&b
		// based on entry being 'activated'
		static 
		void 
		set_gamma(GtkWidget *entry, gpointer data);

		// select which graphene lattice to use, 2 atoms or 4
		// based on entry being 'activated'
		static 
		void 
		set_graphene_basis(GtkWidget *entry, gpointer data);

		// define the c/a ratio for building a graphene sample
		// based on entry being 'activated'
		static 
		void 
		set_cta(GtkWidget *entry, gpointer data);

		// open a panel with access to all parameters needed to define
		// a crystal.  
		static 
		void 
		open_advanced(GtkWidget *button, gpointer data);

		// save the output coordinates in fractional units
		static 
		void 
		set_fract(GtkToggleButton *swtch);

		// save the output coordinates in cartesian units (Å)
		static 
		void 
		set_cart(GtkToggleButton *swtch);

		// callback function to open a pop-up window with entries 
		// required for defining the properties of a basis atom
		static
		void
		add_atom_window(GtkWidget *widget, gpointer data);

		// call back function to respond when a user hits the enter
		// button after defining the basis.  Adds the coordinates to
		// the vector of customBasis
		static
		void
		add_atom(GtkWidget *entry, gpointer data);

		// call back function to clear the contents of std::vector customBasis 
		static
		void
		reset_basis(GtkWidget *widget, gpointer data);

};
#endif 
