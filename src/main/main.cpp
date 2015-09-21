#include "interface.h"
#include "../gui/gui.h"
#include "../gui/render.h"

// version 3.3.0


int main( int nargs, char **argv)
{
    // start up the gui
    Gui *gui = new Gui();

    // start up all the classes
    INTERFACE *interface = new INTERFACE(nargs,argv,NULL);

    // if the app was not built with gtk+, then leave
    #ifndef USE_GTK
        char mesg[MAX_STRING_LEGNTH];
        sprintf(mesg, "%s was not built with USE_GTK, currently, the command line interface is not supported, exiting now\n",argv[0]);
        interface->errors->kill(FERR,"MAIN::set_up()", LERR, mesg);
    #endif

    // force Open GL to grab the args first,
    // weird things happen if not ??
    #ifdef OGL
        glutInit(&nargs, argv);
    #endif

    

    // if there are no arguments passed, assume the user wants the gui
    if (nargs<2 || strcmp(argv[1],"-h")!=0) {
        
        // initialize the gui environment
        gtk_init(&nargs, &argv);

        // define the main window
        GtkWidget *main_window;

        // bring up the gui
        interface->gui->display_in(main_window);

        // wait for the user to do something
        gtk_main();

        // store the settings enterd during the gui session
        interface->gui->save_settings();
        if (interface->insane()) {
            interface->errors->kill(FERR,"MAIN::set_up()", LERR, "Not all crystal parameters were set properly, exiting now\n");
        } else {
 
            if (DMSG) interface->show_parameters();

            interface->build();

            // if the user wants to view the crystal, load the render
            // engine with the appropriate data
            if (interface->gui->view) {

                //make sure the Open GL library/framework was loaded
                #ifdef OGL
                    gl_init();
                    gl_menu_init();

                    std::vector<std::vector<double> > latticeVectors;

                    latticeVectors.push_back(interface->lattice->a1);
                    latticeVectors.push_back(interface->lattice->a2);
                    latticeVectors.push_back(interface->lattice->a3);

                    render_init(interface->gui->out_name,
                                interface->lattice->natom,
                                interface->lattice->ntype,
                                latticeVectors,
                                interface->lattice->type_list,
                                interface->lattice->coords,
                                interface->lattice->species);


                    interface->write_data();
                    glutMainLoop();
                #endif
            } else {
                // if the user didn't hit the render button, just build the crystal and move on
                interface->write_data();
            }
            
            if (GMSG) 
                printf ("MAIN::EXE    >>> Success, exiting now!\n" );
        }
    } 
 


    delete interface;
    
    //leave
    return 0;
}
