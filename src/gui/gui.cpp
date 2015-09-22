#include "gui.h"

// parameter file to define the crystal
char *g_pfile;
// file name to write
char *g_out_name;
// type of file to write
char *g_file_format;
// which lattice ?
char *g_bravais;
char *g_lstyle;
char *g_pd_struct;

// species for defining a single atom basis
char *g_element;

// latttice parameter a (Å)
double g_alat;
double g_blat;
double g_clat;

// lattice angles (˚)
double g_alpha;
double g_beta;
double g_gamma;

// reset count, for debugging
int g_resets;
// run count, for debugging
int g_run_count;

// supercell extensions
int g_nx;
int g_ny;
int g_nz;

// quit signal
int g_quit;

int g_view;

int g_fract;

// graphene globals
int g_graphene;
// c/a ratio
double g_cta;
// number of basis atoms
int g_lbasis;

int g_custom;

std::string elements[110] = 
    {"EM",
     "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", 
     "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", 
     "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn",
     "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", 
     "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn",
     "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd",
     "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb",
     "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg",
     "Ti", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th",
     "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm",
     "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt"};

std::string formats[3] = { "dmol (.car)", "lammps (.input)", "vasp (POSCAR)"};

std::string crystals[6] = {"Cubic", "Hexagonal", "Monoclinic", "Orthorhombic", "Tetragonal", "Library"};

// container for the text entry of a basis atoms coordinates
std::vector<std::string> basisList;

// string to hold the custom stoichiometry
std::string g_entryString;


/*
--
Constructor launches an instances of the parser class to handle the string maniupulations
It also defines all of the global variables to a reasonable value so that the app can be
run with as little interaction as possible
--
*/
Gui::Gui()
{
    parser = new Parser();

    g_graphene = 0;
    g_custom = 0;
    g_fract = 0;
    g_quit = 0;
    g_view = 0;

    view = 0;
    g_resets = 0;
    g_run_count = -1;
    g_nx = g_ny = g_nz = 1;

    g_pd_struct = NULL;
    g_out_name = NULL;
    g_file_format = NULL;
    g_bravais = NULL;
    g_lstyle = NULL;
    g_element = NULL;

    g_alat = g_blat = g_clat = 1;
    g_alpha = g_beta = g_gamma = 90;
    g_cta = 1;
    g_lbasis = 2;

};

Gui::~Gui()
{
    delete parser;
};



// functions related to creating/manipulating/storing
// data for/from the frontend
void 
Gui::display_in(GtkWidget *window)
{

    GtkWidget *button;
    GtkWidget *table;
    GtkWidget *notebook;
    GtkWidget *frame;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *entry;
    GtkWidget *label;
    GtkWidget *chk_cart, *chk_fract;
    GtkWidget *chk_xmgr;
    GtkWidget *checkbutton;
    GtkWidget *dropdown;

    
    int i;
    char bufferf[32];
    char bufferl[32];
    char windowTitle[50];
    sprintf(bufferf, "::::");

    char message[MAX_STRING_LENGTH];


    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    g_signal_connect (window, "delete-event", G_CALLBACK (gtk_main_quit), NULL);

    gtk_container_set_border_width (GTK_CONTAINER (window), 10);


    sprintf(windowTitle,"%s v. %s",SOFT,VERS);

    gtk_window_set_title (GTK_WINDOW (window), windowTitle);

    table = gtk_table_new (3, 6, FALSE);
    gtk_container_add (GTK_CONTAINER (window), table);

    /* Create a new notebook, place the position of the tabs */
    notebook = gtk_notebook_new ();
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
    gtk_table_attach_defaults (GTK_TABLE (table), notebook, 0, 6, 0, 1);
    gtk_widget_show (notebook);

    /////////////////////////////////////////////////////////////////////////////

    sprintf(bufferl, "Lattice");

    frame = gtk_frame_new (bufferf);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
    gtk_widget_set_size_request (frame, FRWIDTH, FRHEIGHT);
    gtk_widget_show (frame);

    vbox = gtk_vbox_new(TRUE,0);
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 100);

    button = gtk_button_new_with_label ("Import file...");
    gtk_box_pack_start(GTK_BOX(vbox),button,1,1,0);
    g_signal_connect(button, "clicked",G_CALLBACK (open_browser),window);

    label = gtk_label_new("Crystal");
    dropdown = fill_dropdown("crystal");
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start(GTK_BOX(hbox),dropdown,1,1,0);
    gtk_widget_set_size_request(dropdown,20,20);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 15);
    /* Connect signal to our handler function */
    g_signal_connect(dropdown, "changed", G_CALLBACK(set_bravais), NULL);

    hbox = gtk_hbox_new (FALSE, 15);
    button = gtk_button_new_with_label ("Custom build...");

    gtk_box_pack_start(GTK_BOX(hbox),button,1,1,0);
    gtk_widget_set_size_request (hbox, 40,40);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 15);
    g_signal_connect(button, "clicked",G_CALLBACK (open_advanced),window);
    gtk_widget_set_tooltip_text(button,"Define the basis an stoichiometry to build a custom crystal");

    gtk_container_add( GTK_CONTAINER( frame ), vbox );
    label = gtk_label_new (bufferl);
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
    /////////////////////////////////////////////////////////////////////////////


    sprintf(bufferl, "Modify");
    frame = gtk_frame_new (bufferf);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 15);
    gtk_widget_set_size_request (frame, FRWIDTH, FRHEIGHT);
    gtk_widget_show (frame);

    vbox = gtk_vbox_new(FALSE,5);
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 30);
    /////////////////////////////////////////////////////////////

    label = gtk_label_new("Atomic Species");
    // load the menu with atomic symbols
    dropdown = fill_dropdown("atoms");
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start(GTK_BOX(hbox),dropdown,1,1,0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 5);
    /* Connect signal to our handler function */
    g_signal_connect(dropdown, "changed",G_CALLBACK(set_atom_species), NULL);

    label = gtk_label_new("\n\n\nSupercell");
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 5);
    // gtk_widget_set_size_request (label, 200,200);

    // Periodic Extensions: nx [_] ny [_] nz [_]
    label = gtk_label_new("nx:");
    GtkWidget *e1, *e2, *e3;
    GtkWidget *l1, *l2, *l3;
    e1 = gtk_entry_new();
    l2 = gtk_label_new("ny:");
    e2 = gtk_entry_new();
    l3 = gtk_label_new("nz:");
    e3 = gtk_entry_new();
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_widget_set_size_request (label, 5,5);
    gtk_box_pack_start(GTK_BOX(hbox),e1,1,1,0);
    gtk_widget_set_size_request (e1, 10,20);
    gtk_box_pack_start(GTK_BOX(hbox),l2,1,1,0);
    gtk_widget_set_size_request (l1, 10,5);
    gtk_box_pack_start(GTK_BOX(hbox),e2,1,1,0);
    gtk_widget_set_size_request (e2, 10,20);
    gtk_box_pack_start(GTK_BOX(hbox),l3,1,1,0);
    gtk_widget_set_size_request (l3, 10,5);
    gtk_box_pack_start(GTK_BOX(hbox),e3,1,1,0);
    gtk_widget_set_size_request (e3, 10,20);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 5);
    g_signal_connect(e1, "activate",G_CALLBACK(set_nx), e1);
    g_signal_connect(e2, "activate",G_CALLBACK(set_ny), e2);
    g_signal_connect(e3, "activate",G_CALLBACK(set_nz), e3);


    hbox = gtk_hbox_new (FALSE, 15);
    button = gtk_button_new_with_label ("Advanced...");

    gtk_box_pack_start(GTK_BOX(hbox),button,1,1,0);
    gtk_widget_set_size_request (hbox, 40,40);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 15);
    g_signal_connect(button, "clicked",G_CALLBACK (open_advanced),window);

    gtk_container_add(GTK_CONTAINER(frame),vbox);

    label = gtk_label_new (bufferl);
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
    /////////////////////////////////////////////////////////////////////////////

    sprintf(bufferl, "Output");

    GtkWidget *vbox1, *vbox2, *vbox3;
    GtkWidget *mvbox;

    mvbox = gtk_vbox_new(FALSE,5);
    gtk_container_set_border_width (GTK_CONTAINER (mvbox), 10);

    frame = gtk_frame_new (bufferf);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
    gtk_widget_set_size_request (frame, FRWIDTH, FRHEIGHT);
    gtk_widget_show (frame);
    vbox1 = gtk_vbox_new(FALSE,5);
    gtk_container_set_border_width (GTK_CONTAINER (vbox1), 30);

    label = gtk_label_new("Format");
    // load the formats
    dropdown = fill_dropdown("formats");

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start(GTK_BOX(hbox),dropdown,1,1,0);
    gtk_box_pack_start (GTK_BOX (vbox1), hbox, FALSE, TRUE, 5);
    /* Connect signal to our handler function */
    g_signal_connect(dropdown, "changed",G_CALLBACK(set_write_style), NULL);

    vbox2 = gtk_vbox_new(FALSE,5);
    gtk_container_set_border_width (GTK_CONTAINER (vbox2), 100);
    button = gtk_button_new_with_label ("Save as...");
    gtk_box_pack_start(GTK_BOX(vbox2),button,1,1,0);
    gtk_widget_set_size_request (button, 10,20);
    g_signal_connect(button, "clicked",G_CALLBACK (save_browser),window);


    hbox = gtk_hbox_new (FALSE, 0);
    label = gtk_label_new("fractional");
    chk_fract = gtk_check_button_new();
    gtk_widget_set_usize(chk_fract, 100, 75);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start(GTK_BOX(hbox),chk_fract,1,1,0);
    g_signal_connect(chk_fract, "toggled",G_CALLBACK(set_fract), NULL);

    label = gtk_label_new("cartesian");
    chk_cart = gtk_check_button_new();
    gtk_widget_set_usize(chk_cart, 100, 75);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start(GTK_BOX(hbox),chk_cart,1,1,0);

    gtk_box_pack_start (GTK_BOX (vbox2), hbox, FALSE, TRUE, 5);
    g_signal_connect(chk_cart, "toggled",G_CALLBACK(set_cart), NULL);

    gtk_box_pack_start (GTK_BOX (mvbox), vbox1, FALSE, TRUE, 5);
    gtk_box_pack_start (GTK_BOX (mvbox), vbox2, FALSE, TRUE, 5);

    gtk_container_add(GTK_CONTAINER(frame),mvbox);

    label = gtk_label_new (bufferl);
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
    /////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////

    sprintf(bufferl, "Help");

    frame = gtk_frame_new (bufferf);
    gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
    gtk_widget_set_size_request (frame, FRWIDTH, FRHEIGHT);
    gtk_widget_show (frame);

    char mesg[] = "=============================================================\n" \
                          "                                        Crystal Builder v. 3.5.0                   \n\n" \
                          "            This is a utility designed to construct various common crystal \n" \
                          "        structures and produce them in the format of several popular simulation\n" \
                          "        packages.  \n" \
                          "      \n\n" \
                          "        This program can be used in one of two ways from this graphical user\n" \
                          "        interface; (1) reading crystal info from a paramters file or (2) defining\n" \
                          "        the criteria listed in the (Lattice,Modify,Outuput) tabs.\n" \
                          "      \n\n" \
                          "        Once a crystal has been defined, you can either select 'Build' which \n" \
                          "        will simply write the requested data to a file.  Alternatively, you can\n" \
                          "        select 'Render+build' to view the crystal.  This option is valid iff\n" \
                          "        this package was built with the Open GL/GLUT libraries!\n"
                          "      \n\n" \
                          "        Crystal structures....: Cubic(BCC (I), FCC (F), Primitive (P), Diamond (I+F)),\n"\
                          "                                Tetragonal(BCC (I), FCC (F), Primitive (P)),\n" \
                          "                                Orthorombic(BCC (I), FCC (F), Primitive (P)),\n"\
                          "                                Graphene(hexagonal, armchair->[100], armchair->[010])\n" \
                          "                                Structures (Load some predefined strcutures)\n"
                          "        File formats..........: DMol, exciting, LAMMPS, VASP\n" \
                          "        Modifications.........: Create supercell, define custom stoichiometry\n"
                          "=============================================================\n";

    label = gtk_label_new(mesg);

    gtk_container_add (GTK_CONTAINER (frame), label);
    gtk_widget_show (label);

    label = gtk_label_new (bufferl);
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
    /////////////////////////////////////////////////////////////////////////////

    /* Set what page to start at (page 1) == Input */
    gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), 0);

    /* Create a bunch of buttons */
    // build the defined crystal
    button = gtk_button_new_with_label ("Build");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (build_crystal), NULL);
    gtk_table_attach_defaults (GTK_TABLE (table), button, 0, 1, 1, 2);
    gtk_widget_show (button);

    button = gtk_button_new_with_label ("Render+build");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (render_crystal), NULL);
    gtk_table_attach_defaults (GTK_TABLE (table), button, 1, 2, 1, 2);
    gtk_widget_show (button);

    // reset_inputs
    button = gtk_button_new_with_label ("Reset");
    g_signal_connect_swapped (button, "clicked",  G_CALLBACK (reset_parameters), notebook );
    gtk_table_attach_defaults (GTK_TABLE (table), button, 2, 3, 1, 2);
    gtk_widget_show (button);

    // view the built crystal
    button = gtk_button_new_with_label ("Quit");
    g_signal_connect_swapped (button, "clicked",  G_CALLBACK (quit_main),  NULL);
    gtk_table_attach_defaults (GTK_TABLE (table), button, 3, 4, 1, 2);
    gtk_widget_show (button);


    gtk_widget_show_all(window);
}

// END MAIN //


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// DIALOGS //
void 
Gui::add_atom_window(GtkWidget *widget, gpointer data)
{
    char windowTitle[50];
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

    gtk_widget_set_size_request (window, 200, 200);

    sprintf(windowTitle,"Add new atom");

    gtk_window_set_title (GTK_WINDOW (window), windowTitle);

    GtkWidget *hbox, *vbox, *button, *entry, *label, *dropdown;

    vbox = gtk_vbox_new(TRUE,0);

    label = gtk_label_new("\n\nAtomic Postions");
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 5);


    GtkWidget *e1;
        // label = gtk_label_new("element");
        // e1 = gtk_entry_new();
        // hbox = gtk_hbox_new (FALSE, 0);
        // gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
        // gtk_widget_set_size_request (label, 5,5);
        // gtk_box_pack_start(GTK_BOX(hbox),e1,1,1,0);
        // gtk_widget_set_size_request (e1, 10,20);
        // gtk_widget_set_tooltip_text(e1, "Enter the elemental symbol");
        // gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 5);
        // g_signal_connect(e1, "activate",G_CALLBACK(set_custom_species), e1);

    label = gtk_label_new("atomic positions");
    e1 = gtk_entry_new();
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_widget_set_size_request (label, 5,5);
    gtk_box_pack_start(GTK_BOX(hbox),e1,1,1,0);
    gtk_widget_set_size_request (e1, 10,20);
    gtk_widget_set_tooltip_text(e1, "Enter the fractional positions separated by commas \n i.e. x,y,z");
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 5);
    g_signal_connect(e1, "activate",G_CALLBACK(add_atom), e1);

    hbox = gtk_hbox_new (FALSE, 15);
    button = gtk_button_new_with_label ("Reset");

    gtk_box_pack_start(GTK_BOX(hbox),button,1,1,0);
    gtk_widget_set_size_request (hbox, 40,40);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 15);
    g_signal_connect(button, "clicked",G_CALLBACK (reset_basis),window);
    gtk_widget_set_tooltip_text(button, "Reset all basis atom positions entered so far,\nall other parameters left unchanged");

    g_custom = 1;

    gtk_container_add( GTK_CONTAINER( window ), vbox );

    gtk_widget_show_all(window);

}

void 
Gui::open_advanced(GtkWidget *widget, gpointer data)
{
    char windowTitle[50];
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

    gtk_widget_set_size_request (window, 400, 300);

    sprintf(windowTitle,"Custom Options");

    gtk_window_set_title (GTK_WINDOW (window), windowTitle);

    GtkWidget *hbox, *vbox, *button, *entry, *label, *dropdown;

    vbox = gtk_vbox_new(TRUE,0);

    label = gtk_label_new("\n\nLattice Parameters");
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 5);

    GtkWidget *e1, *e2, *e3;
    GtkWidget *l1, *l2, *l3;

    label = gtk_label_new("a (Å)");
    e1 = gtk_entry_new();
    l2 = gtk_label_new("b (Å)");
    e2 = gtk_entry_new();
    l3 = gtk_label_new("c (Å)");
    e3 = gtk_entry_new();
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_widget_set_size_request (label, 5,5);
    gtk_box_pack_start(GTK_BOX(hbox),e1,1,1,0);
    gtk_widget_set_size_request (e1, 10,20);
    gtk_box_pack_start(GTK_BOX(hbox),l2,1,1,0);
    gtk_widget_set_size_request (l1, 10,5);
    gtk_box_pack_start(GTK_BOX(hbox),e2,1,1,0);
    gtk_widget_set_size_request (e2, 10,20);
    gtk_box_pack_start(GTK_BOX(hbox),l3,1,1,0);
    gtk_widget_set_size_request (l3, 10,5);
    gtk_box_pack_start(GTK_BOX(hbox),e3,1,1,0);
    gtk_widget_set_size_request (e3, 10,20);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 5);
    g_signal_connect(e1, "activate",G_CALLBACK(set_alat), e1);
    g_signal_connect(e2, "activate",G_CALLBACK(set_blat), e2);
    g_signal_connect(e3, "activate",G_CALLBACK(set_clat), e3);


    label = gtk_label_new("α (°)");
    e1 = gtk_entry_new();
    
    l2 = gtk_label_new("β (°)");
    e2 = gtk_entry_new();
    l3 = gtk_label_new("γ (°)");
    e3 = gtk_entry_new();
    hbox = gtk_hbox_new (FALSE, 10);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_widget_set_size_request (label, 5,5);
    gtk_box_pack_start(GTK_BOX(hbox),e1,1,1,0);
    gtk_widget_set_size_request (e1, 10,20);
    gtk_box_pack_start(GTK_BOX(hbox),l2,1,1,0);
    gtk_widget_set_size_request (l1, 10,5);
    gtk_box_pack_start(GTK_BOX(hbox),e2,1,1,0);
    gtk_widget_set_size_request (e2, 10,20);
    gtk_box_pack_start(GTK_BOX(hbox),l3,1,1,0);
    gtk_widget_set_size_request (l3, 10,5);
    gtk_box_pack_start(GTK_BOX(hbox),e3,1,1,0);
    gtk_widget_set_size_request (e3, 10,20);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 5);
    g_signal_connect(e1, "activate",G_CALLBACK(set_alpha), e1);
    g_signal_connect(e2, "activate",G_CALLBACK(set_beta), e2);
    g_signal_connect(e3, "activate",G_CALLBACK(set_gamma), e3);

    label = gtk_label_new("\n\nCustom Stoichiometry");
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 5);
    


    e1 = gtk_entry_new();
    hbox = gtk_hbox_new (FALSE, 10);
    gtk_box_pack_start(GTK_BOX(hbox),e1,1,1,0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 5);
    gtk_widget_set_tooltip_text(e1, "Enter the species, in stoichiometric order. \nUse commas to separate individual species, i.e. C:2,H:4,O:3");
    g_signal_connect(e1, "activate",G_CALLBACK(set_custom_species), e1);


    hbox = gtk_hbox_new (FALSE, 15);
    button = gtk_button_new_with_label ("Add atom...");

    gtk_box_pack_start(GTK_BOX(hbox),button,1,1,0);
    gtk_widget_set_size_request (hbox, 40,40);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 15);
    g_signal_connect(button, "clicked",G_CALLBACK (add_atom_window),window);


    gtk_container_add( GTK_CONTAINER( window ), vbox );

    gtk_widget_show_all(window);
}


void 
Gui::structures_dialog()
{
    char windowTitle[50];
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_container_set_border_width (GTK_CONTAINER (window), 60);

    sprintf(windowTitle,"Pre-built crystals");

    gtk_window_set_title (GTK_WINDOW (window), windowTitle);

    GtkWidget *hbox, *vbox, *button, *entry, *label, *dropdown;

    vbox = gtk_vbox_new(TRUE,0);

    label = gtk_label_new("MDC  == Metal Dichalcogenide i.e. (Pb,Sn)(S,Se,Te)2\n\nTMDC == Transition Metal Dichalcogenide i.e. (Mo,W)(S,Se,Te)2");
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 15);


    label = gtk_label_new("2D materials: ");
    dropdown = gtk_combo_box_new_text();
    gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "graphene" );
    gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "h-BN" );
    gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "MDC MX2-2H" );
    // gtk_widget_set_tooltip_text(dropdown, );
    gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "MDC MX2-1T" );

    gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "TMDC MX2-2H" );
    gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "TMDC MX2-1T" );


    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start(GTK_BOX(hbox),dropdown,1,1,0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 15);
    /* Connect signal to our handler function */
    g_signal_connect(dropdown, "changed", G_CALLBACK(set_pd_struct), NULL);


    label = gtk_label_new("Energetic materials: ");
    dropdown = gtk_combo_box_new_text();
    gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "PETN-I" );
    gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "TATB" );
    gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "B-HMX" );
    gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "Nitromethane" );

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start(GTK_BOX(hbox),dropdown,1,1,0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 15);
    /* Connect signal to our handler function */
    g_signal_connect(dropdown, "changed", G_CALLBACK(set_pd_struct), NULL);


    gtk_container_add( GTK_CONTAINER( window ), vbox );

    gtk_widget_show_all(window);
}

void 
Gui::graphene_dialog()
{
    char windowTitle[50];
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_container_set_border_width (GTK_CONTAINER (window), 60);

    sprintf(windowTitle,"Graphene settings");

    gtk_window_set_title (GTK_WINDOW (window), windowTitle);

    GtkWidget *hbox, *vbox, *button, *entry, *label;

    label = gtk_label_new("Basis atoms");
    entry = gtk_entry_new();

    hbox = gtk_hbox_new (FALSE, 0);
    vbox = gtk_vbox_new(TRUE,0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start(GTK_BOX(hbox),entry,1,1,0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 15);
    g_signal_connect(entry, "activate",G_CALLBACK(set_graphene_basis), entry);
    gtk_widget_set_tooltip_text(entry,"Enter 2 for hexagonal, 4 for rectangular");

    label = gtk_label_new("c/a ratio");
    entry = gtk_entry_new();

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start(GTK_BOX(hbox),entry,1,1,0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 15);
    g_signal_connect(entry, "activate",G_CALLBACK(set_cta), entry);


    label = gtk_label_new("lattice parameter 'a' (Å)");
    entry = gtk_entry_new();
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start(GTK_BOX(hbox),entry,1,1,0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 15);
    g_signal_connect(entry, "activate",G_CALLBACK(set_alat), entry);
    gtk_widget_set_tooltip_text(entry,"a(Å) = sqrt(3) * rcc");


    gtk_container_add( GTK_CONTAINER( window ), vbox );

    gtk_widget_show_all(window);

    g_graphene = 1;
}

void 
Gui::open_dialog(const char *syst, char message[MAX_STRING_LENGTH])
{
    char windowTitle[50];
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    GtkWidget *hbox, *vbox, *button, *entry, *label, *dropdown;

    gtk_container_set_border_width (GTK_CONTAINER (window), 80);

    if (strncmp(syst,"Cubic",5)==0) {
        sprintf(windowTitle,"Cubic settings");

        gtk_window_set_title (GTK_WINDOW (window), windowTitle);
        gtk_container_set_border_width (GTK_CONTAINER (window), 15);
        gtk_widget_set_size_request (window, 300, 300);

        
        vbox = gtk_vbox_new(TRUE,0);

        label = gtk_label_new("Crystal");
        dropdown = gtk_combo_box_new_text();
        gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "BCC" );
        gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "FCC" );
        gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "SC" );
        gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "Diamond" );

        hbox = gtk_hbox_new (FALSE, 0);
        gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
        gtk_box_pack_start(GTK_BOX(hbox),dropdown,1,1,0);
        gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 15);
        /* Connect signal to our handler function */
        g_signal_connect(dropdown, "changed", G_CALLBACK(set_lattice_style), NULL);


    } else if (strncmp(syst,"Tetragonal",10)==0) {
        sprintf(windowTitle,"Tetragonal settings");

        gtk_window_set_title (GTK_WINDOW (window), windowTitle);
        gtk_container_set_border_width (GTK_CONTAINER (window), 15);
        gtk_widget_set_size_request (window, 300, 300);
        vbox = gtk_vbox_new(TRUE,0);

        label = gtk_label_new("Crystal");
        dropdown = gtk_combo_box_new_text();
        gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "BCC" );
        gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "FCC" );
        gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "SC" );
        gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "Diamond" );

        hbox = gtk_hbox_new (FALSE, 0);
        gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
        gtk_box_pack_start(GTK_BOX(hbox),dropdown,1,1,0);
        gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 15);
        /* Connect signal to our handler function */
        g_signal_connect(dropdown, "changed", G_CALLBACK(set_lattice_style), NULL);


    } else if (strncmp(syst,"Orthorhombic",12)==0) {
        sprintf(windowTitle,"Orthorhombic settings");

        gtk_window_set_title (GTK_WINDOW (window), windowTitle);
        gtk_container_set_border_width (GTK_CONTAINER (window), 15);
        gtk_widget_set_size_request (window, 300, 300);

        vbox = gtk_vbox_new(TRUE,0);

        label = gtk_label_new("Crystal");
        dropdown = gtk_combo_box_new_text();
        gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "BCC" );
        gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "FCC" );
        gtk_combo_box_append_text( GTK_COMBO_BOX( dropdown ), "SC" );

        hbox = gtk_hbox_new (FALSE, 0);
        gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
        gtk_box_pack_start(GTK_BOX(hbox),dropdown,1,1,0);
        gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 15);
        /* Connect signal to our handler function */
        g_signal_connect(dropdown, "changed", G_CALLBACK(set_lattice_style), NULL);
    }

    label = gtk_label_new("\n\nLattice Parameters");
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 5);

    GtkWidget *e1, *e2, *e3;
    GtkWidget *l1, *l2, *l3;

    label = gtk_label_new("a (Å)");
    e1 = gtk_entry_new();
    l2 = gtk_label_new("b (Å)");
    e2 = gtk_entry_new();
    l3 = gtk_label_new("c (Å)");
    e3 = gtk_entry_new();
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_widget_set_size_request (label, 5,5);
    gtk_box_pack_start(GTK_BOX(hbox),e1,1,1,0);
    gtk_widget_set_size_request (e1, 10,20);
    gtk_box_pack_start(GTK_BOX(hbox),l2,1,1,0);
    gtk_widget_set_size_request (l1, 10,5);
    gtk_box_pack_start(GTK_BOX(hbox),e2,1,1,0);
    gtk_widget_set_size_request (e2, 10,20);
    gtk_box_pack_start(GTK_BOX(hbox),l3,1,1,0);
    gtk_widget_set_size_request (l3, 10,5);
    gtk_box_pack_start(GTK_BOX(hbox),e3,1,1,0);
    gtk_widget_set_size_request (e3, 10,20);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 5);
    g_signal_connect(e1, "activate",G_CALLBACK(set_alat), e1);
    g_signal_connect(e2, "activate",G_CALLBACK(set_blat), e2);
    g_signal_connect(e3, "activate",G_CALLBACK(set_clat), e3);


    label = gtk_label_new("α (°)");
    e1 = gtk_entry_new();
    l2 = gtk_label_new("β (°)");
    e2 = gtk_entry_new();
    l3 = gtk_label_new("γ (°)");
    e3 = gtk_entry_new();
    hbox = gtk_hbox_new (FALSE, 10);
    gtk_box_pack_start(GTK_BOX(hbox),label,1,1,0);
    gtk_widget_set_size_request (label, 5,5);
    gtk_box_pack_start(GTK_BOX(hbox),e1,1,1,0);
    gtk_widget_set_size_request (e1, 10,20);
    gtk_box_pack_start(GTK_BOX(hbox),l2,1,1,0);
    gtk_widget_set_size_request (l1, 10,5);
    gtk_box_pack_start(GTK_BOX(hbox),e2,1,1,0);
    gtk_widget_set_size_request (e2, 10,20);
    gtk_box_pack_start(GTK_BOX(hbox),l3,1,1,0);
    gtk_widget_set_size_request (l3, 10,5);
    gtk_box_pack_start(GTK_BOX(hbox),e3,1,1,0);
    gtk_widget_set_size_request (e3, 10,20);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 5);
    g_signal_connect(e1, "activate",G_CALLBACK(set_alpha), e1);
    g_signal_connect(e2, "activate",G_CALLBACK(set_beta), e2);
    g_signal_connect(e3, "activate",G_CALLBACK(set_gamma), e3);

    gtk_container_add( GTK_CONTAINER( window ), vbox );

    gtk_widget_show_all(window);
}

void
Gui::reminder()
{
    GtkWidget *dialog, *window;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Rember to define the stoichiometry and relevant lattice parameters with the `Advanced` button in the `Modify` tab ");
    gtk_window_set_title(GTK_WINDOW(dialog), "Reminder!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


// CALLBACKS //
void 
Gui::set_fract( GtkToggleButton *swtch )
{
    if (swtch->active) {
        if (GMSG) g_print("GUI::MODIFY    >>> [ fractional coordinates ] formatting turned on\n" );
        g_fract = 1;
    } else {
        if (GMSG) g_print("GUI::MODIFY    >>> [ fractional coordinates ] formatting turned off\n" );
        g_fract = 0;
    }
}

void 
Gui::set_cart( GtkToggleButton *swtch )
{
    if (swtch->active) {
        if (GMSG) g_print("GUI::MODIFY    >>> [ cartesian coordinates ] formatting turned on\n" );
        g_fract = 0;
    } else {
        if (GMSG) g_print("GUI::MODIFY    >>> [ cartesian coordinates ] formatting turned off\n" );
        g_fract = 1;
    }
}


void 
Gui::set_bravais( GtkComboBox *dropdown, gpointer data )
{
    /* Obtain currently selected string from dropdown box */
    gchar *selection = gtk_combo_box_get_active_text( dropdown );
    g_bravais = (char*) gtk_combo_box_get_active_text( dropdown);
    /* Print it to the console - if nothing is selected, print NULL */
    if (GMSG) g_print( "GUI::PARAMETER >>  selecting bravais lattice [ %s ]\n", ( selection ? selection : "NULL" ) );

    char message[MAX_STRING_LENGTH] = "Please enter the following:";
    if (strncmp(selection,"Cubic",5)==0) open_dialog("Cubic",message);
    else if (strncmp(selection,"Tetragonal",10)==0) open_dialog("Tetragonal",message);
    else if (strncmp(selection,"Orthorhombic",12)==0) open_dialog("Orthorhombic",message);
    else if (strncmp(selection,"Library",10)==0) structures_dialog();
    /* Free selection */
    g_free( selection );
}

void 
Gui::set_lattice_style( GtkComboBox *dropdown, gpointer data )
{
    /* Obtain currently selected string from dropdown box */
    gchar *selection = gtk_combo_box_get_active_text( dropdown );
    g_lstyle = (char*) gtk_combo_box_get_active_text( dropdown);
    /* Print it to the console - if nothing is selected, print NULL */
    if (GMSG) g_print( "GUI::MODIFY    >>> selecting lattice style [ %s ]\n", ( selection ? selection : "NULL" ) );
    /* Free selection */
    g_free( selection );
}

void 
Gui::set_pd_struct( GtkComboBox *dropdown, gpointer data )
{
    /* Obtain currently selected string from dropdown box */
    gchar *selection = gtk_combo_box_get_active_text( dropdown );
    g_pd_struct = (char*) gtk_combo_box_get_active_text( dropdown);
    /* Print it to the console - if nothing is selected, print NULL */
    if (GMSG) g_print( "GUI::PARAMETER >>  using a pre-defined structure [ %s ]\n", ( selection ? selection : "NULL" ) );

    if (strncmp(g_pd_struct,"graphene",8)==0) graphene_dialog();

    if (strncmp(g_pd_struct,"TMDC MX2-2H",11)==0 || strncmp(g_pd_struct,"TMDC MX2-1T",11)==0) {
       reminder();
    } 

    if (strncmp(g_pd_struct,"MDC MX2-2H",11)==0 || strncmp(g_pd_struct,"MDC MX2-1T",11)==0) {
       reminder();
    } 
    

    /* Free selection */
    g_free( selection );
}


void 
Gui::set_custom_species(GtkWidget *entry, gpointer data)
{
    
    std::string entryList((char*) gtk_entry_get_text(GTK_ENTRY(data)));
    
    g_entryString = entryList;

    if (GMSG) g_print("GUI::MODIFY    >>> defining custom stoichiometry: %s\n",entryList.c_str());

    gtk_editable_select_region(GTK_EDITABLE(data), 0,-1); // text from 0 to the end
    gtk_editable_copy_clipboard(GTK_EDITABLE(data));


}

void 
Gui::set_alat(GtkWidget *widget, gpointer data)
{
    if (GMSG) g_print("GUI::MODIFY    >>> setting a lattice parameter [ %s ]\n", gtk_entry_get_text(GTK_ENTRY(data)));
    g_alat = atof((char*) gtk_entry_get_text(GTK_ENTRY(data)));
    gtk_editable_select_region(GTK_EDITABLE(data), 0,-1); // text from 0 to the end
    gtk_editable_copy_clipboard(GTK_EDITABLE(data));
}

void 
Gui::set_blat(GtkWidget *widget, gpointer data)
{
    if (GMSG) g_print("GUI::MODIFY    >>> setting b lattice parameter [ %s ]\n", gtk_entry_get_text(GTK_ENTRY(data)));
    g_blat = atof((char*) gtk_entry_get_text(GTK_ENTRY(data)));
    gtk_editable_select_region(GTK_EDITABLE(data), 0,-1); // text from 0 to the end
    gtk_editable_copy_clipboard(GTK_EDITABLE(data));
}

void 
Gui::set_clat(GtkWidget *widget, gpointer data)
{
    if (GMSG) g_print("GUI::MODIFY    >>> setting c lattice parameter [ %s ]\n", gtk_entry_get_text(GTK_ENTRY(data)));
    g_clat = atof((char*) gtk_entry_get_text(GTK_ENTRY(data)));
    gtk_editable_select_region(GTK_EDITABLE(data), 0,-1); // text from 0 to the end
    gtk_editable_copy_clipboard(GTK_EDITABLE(data));
}

void 
Gui::set_alpha(GtkWidget *widget, gpointer data)
{
    if (GMSG) g_print("GUI::MODIFY    >>> setting alpha [ %s° ]\n", gtk_entry_get_text(GTK_ENTRY(data)));
    g_alpha = atof((char*) gtk_entry_get_text(GTK_ENTRY(data)));
    gtk_editable_select_region(GTK_EDITABLE(data), 0,-1); // text from 0 to the end
    gtk_editable_copy_clipboard(GTK_EDITABLE(data));
}

void 
Gui::set_beta(GtkWidget *widget, gpointer data)
{
    if (GMSG) g_print("GUI::MODIFY    >>> setting beta [ %s° ]\n", gtk_entry_get_text(GTK_ENTRY(data)));
    g_beta = atof((char*) gtk_entry_get_text(GTK_ENTRY(data)));
    gtk_editable_select_region(GTK_EDITABLE(data), 0,-1); // text from 0 to the end
    gtk_editable_copy_clipboard(GTK_EDITABLE(data));
}

void 
Gui::set_gamma(GtkWidget *widget, gpointer data)
{
    if (GMSG) g_print("GUI::MODIFY    >>> setting gamma [ %s° ]\n", gtk_entry_get_text(GTK_ENTRY(data)));
    g_gamma = atof((char*) gtk_entry_get_text(GTK_ENTRY(data)));
    gtk_editable_select_region(GTK_EDITABLE(data), 0,-1); // text from 0 to the end
    gtk_editable_copy_clipboard(GTK_EDITABLE(data));
}


void 
Gui::set_graphene_basis(GtkWidget *widget, gpointer data)
{
    g_lbasis = atoi((char*) gtk_entry_get_text(GTK_ENTRY(data)));
    if (g_lbasis == 2 || g_lbasis == 4) {
        if (GMSG) g_print("GUI::MODIFY    >>> setting number of basis atoms [ %s ]\n", gtk_entry_get_text(GTK_ENTRY(data)));
    } else {
        fprintf (WMESG,"GUI::WARNING   >>   Unknown basis atoms, enter 2 or 4\n");
        fprintf (WMESG,"                    Basis atoms not set, try again\n");
    }
    gtk_editable_select_region(GTK_EDITABLE(data), 0,-1); // text from 0 to the end
    gtk_editable_copy_clipboard(GTK_EDITABLE(data));
}

void 
Gui::set_cta(GtkWidget *widget, gpointer data)
{
    if (GMSG) g_print("GUI::MODIFY    >>> setting c/a [ %s ]\n", gtk_entry_get_text(GTK_ENTRY(data)));
    g_cta = atof((char*) gtk_entry_get_text(GTK_ENTRY(data)));
    gtk_editable_select_region(GTK_EDITABLE(data), 0,-1); // text from 0 to the end
    gtk_editable_copy_clipboard(GTK_EDITABLE(data));
}

void 
Gui::set_atom_species( GtkComboBox *dropdown, gpointer data )
{
    /* Obtain currently selected string from dropdown box */
    gchar *selection = gtk_combo_box_get_active_text( dropdown );
    g_element = (char*) gtk_combo_box_get_active_text( dropdown);
    /* Print it to the console - if nothing is selected, print NULL */
    if (GMSG) g_print( "GUI::PARAMETER >>  atomic species [ %s ]\n", ( selection ? selection : "NULL" ) );
    /* Free selection */
    g_free( selection );
}

void 
Gui::set_write_style( GtkComboBox *dropdown, gpointer data )
{
    /* Obtain currently selected string from dropdown box */
    gchar *selection = gtk_combo_box_get_active_text( dropdown );
    g_file_format = (char*) gtk_combo_box_get_active_text( dropdown);
    /* Print it to the console - if nothing is selected, print NULL */
    if (GMSG) g_print( "GUI::PARAMETER >>  writing file for [ %s ]\n", ( selection ? selection : "NULL" ) );
    /* Free selection */
    g_free( selection );
}


void 
Gui::set_nx(GtkWidget *widget, gpointer data)
{
    if (GMSG) g_print("GUI::MODIFY    >>> setting nx to [ %s ]\n", gtk_entry_get_text(GTK_ENTRY(data)));
    g_nx = atoi((char*)gtk_entry_get_text(GTK_ENTRY(data)));
    gtk_editable_select_region(GTK_EDITABLE(data), 0,-1); // text from 0 to the end
    gtk_editable_copy_clipboard(GTK_EDITABLE(data));
}

void 
Gui::set_ny(GtkWidget *widget, gpointer data)
{
    if (GMSG) g_print("GUI::MODIFY    >>> setting ny to [ %s ]\n", gtk_entry_get_text(GTK_ENTRY(data)));
    g_ny = atoi((char*)gtk_entry_get_text(GTK_ENTRY(data)));
    gtk_editable_select_region(GTK_EDITABLE(data), 0,-1); // text from 0 to the end
    gtk_editable_copy_clipboard(GTK_EDITABLE(data));
}

void 
Gui::set_nz(GtkWidget *widget, gpointer data)
{
    if (GMSG) g_print("GUI::MODIFY    >>> setting nz to [ %s ]\n", gtk_entry_get_text(GTK_ENTRY(data)));
    g_nz = atoi((char*)gtk_entry_get_text(GTK_ENTRY(data)));
    gtk_editable_select_region(GTK_EDITABLE(data), 0,-1); // text from 0 to the end
    gtk_editable_copy_clipboard(GTK_EDITABLE(data));
}

void 
Gui::open_browser(GtkWidget* button, gpointer window)
{
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Chosse a file", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_widget_show_all(dialog);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), g_get_current_dir());
    gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
    if(resp == GTK_RESPONSE_OK) {
        if (GMSG) g_print("GUI::PARAMETER >>  Reading from logfile [ %s ]....\n", gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
        g_pfile = (char*)gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    }   
    else {
        if (GMSG) g_print("GUI::PARAMETER >>  Operation cancelled!\n");
    }
    gtk_widget_destroy(dialog);
}

void 
Gui::save_browser(GtkWidget* button, gpointer window)
{
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Chosse a file", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_widget_show_all(dialog);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), g_get_current_dir());
    gint resp = gtk_dialog_run(GTK_DIALOG(dialog));

    const char *fpath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

    std::string fqp(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
    std::size_t found = fqp.find_last_of("/");

    if(resp == GTK_RESPONSE_OK) {
        if (GMSG) g_print("GUI::PARAMETER >>  Saving file as [ %s ]....\n", fqp.substr(found+1).c_str());
        g_out_name = (char*)gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    }   
    else {
        if (GMSG) g_print("GUI::PARAMETER >>  Operation cancelled!\n");
    }
    gtk_widget_destroy(dialog);
}

void
Gui::reset_basis(GtkWidget *widget, gpointer data)
{
    g_print("GUI::EXECUTE   >>> RESET (basis)\n");
    basisList.erase(basisList.begin(),basisList.end());
}

void 
Gui::add_atom(GtkWidget *entry, gpointer data)
{
    std::string entryList((char*) gtk_entry_get_text(GTK_ENTRY(data)));

    basisList.push_back( entryList );

    if (GMSG) g_print("GUI::MODIFY    >>> adding atom to basis position: ( %s )\n",entryList.c_str());

    gtk_editable_select_region(GTK_EDITABLE(data), 0,-1); // text from 0 to the end
    gtk_editable_copy_clipboard(GTK_EDITABLE(data));
}

void
Gui::define_basis_atoms()
{
    std::vector<double> row(3);

    for (int i=0;i<basisList.size();i++) {
        std::vector<std::string> coordinates = parser->split_list(basisList[i],',');
        row[0] = parser->str2num<double>(coordinates[0]);
        row[1] = parser->str2num<double>(coordinates[1]);
        row[2] = parser->str2num<double>(coordinates[2]);
        if (DMSG)
            fprintf (FDEBUG,"GUI::DEBUG::MESG: row %i = %f %f %f \n ",i,row[0],row[1],row[2]);
        customBasis.push_back( row );
    }
}

void 
Gui::reset_parameters(GtkWidget* button, gpointer data)
{
    g_resets++;

    g_print("GUI::EXECUTE   >>> RESET (all)\n");

    g_pd_struct = NULL;
    g_out_name = NULL;
    g_file_format = NULL;
    g_bravais = NULL;
    g_lstyle = NULL;
    g_element = NULL;

    g_alat = g_blat = g_clat = 1;
    g_alpha = g_beta = g_gamma = 90;
    g_cta = 0;
    g_lbasis = 2;

    g_fract = 0;
    g_nx = g_ny = g_nz = 1;
}

void 
Gui::quit_main(GtkWidget *button, gpointer data)
{
    g_quit = 1;
    gtk_main_quit();
}

void 
Gui::render_crystal(GtkWidget *button, gpointer data)
{

    g_view = 1;
    if (GMSG) g_print("GUI::EXECUTE   >> building and rendering crystal\n");
    gtk_main_quit();
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

// AUX //
GtkWidget 
*Gui::fill_dropdown(const char *style)
{
    GtkWidget *full_menu = gtk_combo_box_new_text();
    if (strncmp(style,"atoms",5)==0) {
        for (int n=1;n<110;n++) {
            char *id = parser->str2char(elements[n]);
            gtk_combo_box_append_text( GTK_COMBO_BOX( full_menu ), id );
        }
    } else if (strncmp(style,"formats",7)==0) {
        for (int n=0;n<3;n++) {
            char *id = parser->str2char(formats[n]);
            gtk_combo_box_append_text( GTK_COMBO_BOX( full_menu ), id );
        }
    } else if (strncmp(style,"crystal",7)==0) {
        for (int n=0;n<6;n++) {
            char *id = parser->str2char(crystals[n]);
            gtk_combo_box_append_text( GTK_COMBO_BOX( full_menu ), id );
        }
    }
    return full_menu;
}

void Gui::build_crystal(GtkWidget* button, gpointer window)
{
    g_view = 0;
    if (GMSG) g_print("GUI::EXECUTE   >> building crystal\n");
    gtk_main_quit();
}

void Gui::save_settings()
{
    fractional = g_fract;
    quit = g_quit;
    view = g_view;
    pfile = g_pfile;
    out_name = g_out_name;
    file_format = g_file_format;
    bravais = g_bravais;
    lstyle = g_lstyle;
    element = g_element;
    pd_struct = g_pd_struct;


    alat = g_alat;
    blat = g_blat;
    clat = g_clat;

    alpha = g_alpha;
    beta = g_beta;
    gamma = g_gamma;

    lbasis = g_lbasis;
    cta = g_cta;
    nx = g_nx;
    ny = g_ny;
    nz = g_nz;

    graphene = g_graphene;

    std::vector<std::string> g_elemList;
    std::vector<std::string> g_elemCount;


    custom = g_custom;

    if (custom) {
        define_basis_atoms();
        // parse the custom list of species and stoichiometric coefficients
        parser->scan_list(g_entryString, ',', ':', g_elemList,g_elemCount);
        std::string unit;
        for (int i=0;i<g_elemList.size();i++) {
            // if there is only 1 of a species, print only the element symbol
            if (parser->str2num<int>(g_elemCount[i])==1) unit = g_elemList[i];
            else unit = g_elemList[i] + g_elemCount[i];

            elemList.push_back(g_elemList[i]);
            elemCount.push_back(parser->str2num<int>(g_elemCount[i]));
            customName += unit;
        }
    } else if (strcmp(bravais,"Library")==0) {
        // set the default Library parameters 
        if (strncmp(g_pd_struct,"graphene",8)==0) {
            elemList.push_back("C");
            if (lbasis==2) elemCount.push_back(2);
            else elemCount.push_back(4);
        } else if (strncmp(g_pd_struct,"MDC MX2-2H",10)==0 || strncmp(g_pd_struct,"MDC MX2-1T",10)==0) {
            elemList.push_back("Sn");
            elemList.push_back("S");
            elemCount.push_back(1);
            elemCount.push_back(2);
            alat = 3.684;
        } else if (strncmp(g_pd_struct,"TMDC MX2-2H",11)==0 || strncmp(g_pd_struct,"TMDC MX2-1T",11)==0) {
            elemList.push_back("Mo");
            elemList.push_back("S");
            elemCount.push_back(1);
            elemCount.push_back(2);
            alat = 3.19;
        } else if (strncmp(g_pd_struct,"PETN-I",6)==0) {
            elemList.push_back("C");
            elemList.push_back("H");
            elemList.push_back("N");
            elemList.push_back("O");
            elemCount.push_back(10);
            elemCount.push_back(16);
            elemCount.push_back(8);
            elemCount.push_back(24);
        } else if (strncmp(g_pd_struct,"TATB",4)==0) {
            elemList.push_back("C");
            elemList.push_back("H");
            elemList.push_back("N");
            elemList.push_back("O");
            elemCount.push_back(12);
            elemCount.push_back(12);
            elemCount.push_back(12);
            elemCount.push_back(12);
        }
    } 

    

}
