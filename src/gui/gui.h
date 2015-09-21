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
  * \version 0.8
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
		int           graphene;
		int           custom;
		int           apcell;
		int           ntypes;

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
		std::vector<std::vector<double> > customBasis;
		std::string customName;


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
		// 'atoms' -> load with atomic symbosl 'formats' -> writing formats
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

		static 
		void 
		graphene_dialog();

		static 
		void 
		structures_dialog();

		// add a new atom to the basis
		static
		void
		add_atom_window(GtkWidget *widget, gpointer data);

		static
		void
		set_custom_basis(GtkWidget *entry, gpointer data);

		static
		void
		reset_basis(GtkWidget *widget, gpointer data);

		/* Callback functions to set user inputs */
		static 
		void 
		set_nx(GtkWidget *widget, gpointer data);

		static 
		void 
		set_ny(GtkWidget *widget, gpointer data);

		static 
		void 
		set_nz(GtkWidget *widget, gpointer data);
		
		static 
		void 
		set_atom_species( GtkComboBox *dropdown, gpointer data );
		
		static
		void 
		set_write_style( GtkComboBox *dropdown, gpointer data );
		
		static 
		void 
		set_bravais( GtkComboBox *dropdown, gpointer data );
		
		static 
		void 
		set_lattice_style(GtkComboBox *widget, gpointer data);
		
		static 
		void 
		set_pd_struct( GtkComboBox *dropdown, gpointer data );


		static 
		void 
		set_custom_species(GtkWidget *entry, gpointer data);

		static 
		void 
		set_alat(GtkWidget *widget, gpointer data);

		static 
		void 
		set_blat(GtkWidget *widget, gpointer data);

		static 
		void 
		set_clat(GtkWidget *widget, gpointer data);

		static 
		void 
		set_alpha(GtkWidget *widget, gpointer data);

		static 
		void 
		set_beta(GtkWidget *widget, gpointer data);

		static 
		void 
		set_gamma(GtkWidget *widget, gpointer data);

		static 
		void 
		set_basis(GtkWidget *widget, gpointer data);

		static 
		void 
		set_cta(GtkWidget *widget, gpointer data);

		static 
		void 
		set_out_name(GtkWidget *widget, gpointer data);

		static 
		void 
		quit_main(GtkWidget *widget, gpointer data);

		static 
		void 
		hide_window(GtkWidget *widget, gpointer data);

		static 
		void 
		render_crystal(GtkWidget *widget, gpointer data);

		static 
		void 
		open_advanced(GtkWidget *widget, gpointer data);

		static 
		void 
		set_fract(GtkToggleButton *swtch);

		static 
		void 
		set_cart(GtkToggleButton *swtch);

		static 
		void 
		define_atom(GtkWidget *widget, gpointer data);

		void
		define_basis_atoms();
};
#endif 
