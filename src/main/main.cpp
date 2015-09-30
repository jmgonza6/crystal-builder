#include "interface.h"
#include "../gui/gui.h"
#include "../gui/render.h"

// version 3.7.0


int main( int nargs, char **argv)
{
    // start up the gui
    Gui *gui = new Gui();

    // start up all the classes
    INTERFACE *interface = new INTERFACE(nargs,argv,NULL);

    

    // force Open GL to grab the args first,
    // weird things happen if not ??
    #ifdef OGL
        glutInit(&nargs, argv);
    #endif


    // if no arguments were passed, assume the user wants to run from the gui
    if (nargs==1) {
        // if the app was not built with gtk+, then leave
        #ifndef USE_GTK
            char mesg[MAX_STRING_LEGNTH];
            sprintf(mesg, "%s was not built with USE_GTK, you must use the command line to pass a parameters file, exiting now\n",argv[0]);
            interface->errors->kill(FERR,"MAIN::set_up()", LERR, mesg);
        #endif

        // initialize the gui environment
        gtk_init(&nargs, &argv);

        // define the main window
        GtkWidget *main_window;

        // bring up the gui
        interface->gui->display_in(main_window);

        // wait for the user to do something
        gtk_main();

        if (gui->quit) return 0;

        // store the settings enterd during the gui session
        interface->gui->save_settings();
        if (interface->insane()) {
            interface->errors->kill(FERR,"MAIN::set_up()", LERR, "Not all crystal parameters were set properly in GUI, exiting now\n");
        } else {

            interface->build();

            // if the user wants to view the crystal, load the render
            // engine with the appropriate data
            if (interface->gui->view) {

                //make sure the Open GL library/framework was loaded
                #ifdef OGL
                    gl_init();
                    gl_menu_init();

                    render_init(interface->lattice->natom,
                                interface->lattice->a1,
                                interface->lattice->a2,
                                interface->lattice->a3,
                                interface->lattice->atoms,
                                interface->lattice->species,
                                interface->lattice->typeCount);

                    interface->write_data(1);
                    glutMainLoop();
                #endif
            } else {
                // if the user didn't hit the render button, just build the crystal and move on
                interface->write_data(1);
            }
            
            if (GMSG) 
                printf ("MAIN::EXE    >>> Success, exiting now!\n" );
        }
    
    } else {
        // assume last argument is the file name
        interface->reader->file(argv[nargs-1]);

        // check that all arguments passed were sensible
        if (interface->reader->insane()) {
            interface->errors->kill(FERR,"MAIN::set_up()", LERR, "Not all crystal parameters were set properly in your parameters file, exiting now\n");
        } else {
            interface->build_from_file();

            // if the user wants to view the crystal, load the render
            // engine with the appropriate data
            if (interface->reader->view) {

                //make sure the Open GL library/framework was loaded
                #ifdef OGL
                    gl_init();
                    gl_menu_init();

                    render_init(interface->lattice->natom,
                                interface->lattice->a1,
                                interface->lattice->a2,
                                interface->lattice->a3,
                                interface->lattice->atoms,
                                interface->lattice->species,
                                interface->lattice->typeCount);
                    interface->write_data(2);
                    glutMainLoop();
                #endif
            } else {
                // if the user didn't specify the render run style, just build the crystal and move on
                interface->write_data(2);
            }
            
            if (GMSG) 
                printf ("MAIN::EXE    >>> Success, exiting now!\n" );
        }
    }

    delete interface;
    
    //leave
    return 0;
}
