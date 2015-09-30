/** 
  * \file render.h
  *
  * \b Purpose:
  *  =============
  \verbatim 
  /*****************************************************************************************\
  /  Simple rendering engine to display the crystal built by the user during a GUI session  \
  /                                                                                         \
  /  Borrowed in part from example3/5.cpp of the GLUI tutorials.                            \
  /*****************************************************************************************\
  \endverbatim
  *
  *\b User interaction:
  *  =============
  * \verbatim
  *
  *  Mouse events:
  *     Left mouse            rotate, zoom
  *     Right mouse           menu
  *
  *  Keyboard events:
  *     'A'                   display atoms and unit cell
  *     'a'                   display atoms only
  *     'B'                   display bonds, uniform nearest neighbor cutoff
  *     'b'                   hide bonds
  *     'C'                   increase nearest neighbor cutoff
  *     'c'                   decrease nearest neighbor cutoff
  *     'e'                   exit and save the data from the rendering scene
  *     'o'                   orthographic projection of the crystal
  *     'p'                   perspective projection of the crystal
  *     'r' then LeftButton   rotate the crystal in all 3 dimensions
  *     'W'                   change background to white and bounding box to black
  *     'w'                   change background to black and bounding box to white
  *     'z' then LeftButton   zoom toward/away from the crystal
  *     '0'                   reset orientation of crystal to looking down z toward xy plane
  *     '1'                   view the xz "a-c" plane of the crystal
  *     '2'                   view the yz "b-c" plane of the crystal
  *     '3'                   rotate crystal about x by -45˚, and then -45˚ about z, 'psuedo' perspective view
  *     '+'                   increase the particle radius uniformly
  *     '-'                   decrease the particle radius uniformly
  *
  *     Up arrow              Shift the crystal in positive vertical screen direction
  *     Down arrow            Shift the crystal in negative vertical screen direction
  *     Left arrow            Shift the crystal in negative horizontal screen direction
  *     Right arrow           Shift the crystal in positive horizontal screen direction
  *
  \endverbatim
  * \author Joseph M. Gonzalez
  *
  * \version 0.9
  *
  * \date Sep 27, 2015 19:16:20
  *
  * \b Contact \n    jmgonza6@mail.usf.edu
  *
  */

#ifndef RENDER_H
#define RENDER_H

#include "../util/common.h"

// portability for apple and linux
// no windoze support...
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// decent 4:3 aspect ratio
#define WWIDTH 600       //!< width of the rendering window
#define WHEIGHT 600       //!< height of the rendering window
#define VPORT_SIZE 60     //!< size of the view port, VPORT_SIZExVPORT_SIZE


enum LeftButton {    ROTATE,SCALE,TRANSLATE,DRAG    };

enum {    SHOWALL,ATOMSONLY,BONDS,BLACKB,WHITEB    };


//
// "Public" functions
//

/** \brief Initialize the Open GL and GLUT infrastructure with window 
  * size, lighting, positions, etc.
  *
  */
void 
gl_init();


/** \brief Initialize the right mouse button menu with scene display options
  *
  */
void 
gl_menu_init();

/** \brief Construct a nearest neighbor list for each particle, naive n^2 algorithm
  * \returns `1` if successful, `0` otherwise
  */
int
build_nn_list();

/** \brief Locally store the data entered by the user during the gui session 
  * \param[in] natom - total number of atoms created by lattice->build()
  * \param[in] a1 - major lattice vector, in cartesian coordinates
  * \param[in] a2 - lattice vector, in `xy plane`, in cartesian coordinates
  * \param[in] a3 - lattice vector, in `xyz plane`, in cartesian coordinates
  * \param[in] atomsIn - Structure containing the atomic positions, in cartesian units, and atomic type numbers
  * \param[in] species - 1D vector containing atomic species
  * \param[in] elemCount - 1D vector containing the count of each type in `species`
  */
int
render_init(int                               natom, 
            std::vector<double>               a1,
            std::vector<double>               a2,
            std::vector<double>               a3,
            atom_t*                           atomsIn,
            std::vector<std::string>          species,
            std::vector<int>                  elemCount);




//
// "Private" drawing functions
//

/** \brief Selectively initialize the atomic and or unit cell data from a GUI session
  * Draws the objects and stores the data in a glList.  These lists  make rendering \n
  * much more efficient with a simple call to glCallList()
  * \param[in] list - which list the re-create
  * \arg 1, bounding box only
  * \arg 2, coordinate axes only
  * \arg 3, atomic coordinates only
  * \arg 4, everything
  * \returns \b 1 if successful \b 0 otherwise
  */
int
init_call_lists(int list);

/** \brief Initialize the coordinate axes for the scene\n
  * x = red , y = green, blue = z\n
  * Drawn using GL_LINE_STRIP and stores the data in a glList, `AxesList`.
  */
void
init_axes_list();

/** \brief Initialize the periodic unit cell\n
  * Drawn using GL_LINE_STRIP and stores the data in a glList, `UnitCellList`.
  */
void
init_bbox_list();


/** \brief Initialize the atomic coordinates\n
  *  The particles are drawn as gluSolidSphere() objects and stores \n
  *  the data in a glList, `AtomsList`.
  */
void
init_atoms_list();

/** \brief Render unit cell, cartesian axes, and atoms in all 4 view ports
  * \param[in] vport - in which view port to draw
  */
void
show_all(int vport);

/** \brief Render atoms only in `vport`
  * \param[in] vport - in which view port to draw
  */
void
atoms_only(int vport);

//
// event driven functions "callbacks"
//

/** \brief Reshape the window
  * \param[in] width - current width, from mouse/motion event
  * \param[in] height - current height, from mouse/motion event
  */
void 
reshape_window(int width, 
               int height);

/** \brief Set the display type
  * \param[in] value - how much data to be shown `1` = all `2` = atoms only
  */
void 
set_display_type(int value);

/** \brief Activate/kill main rendering window
  * \param[in] value - use ESC to terminate
  */
void 
kill_main_window(int value);

/** \brief Set the display type, the projection type, initialize call lists, and handle scene modifications\n
  *  Called by `glutDisplayFunc()`.
  */
void 
render_data();


/** \brief Draw a solid cylinder between nearest neighbors
  * \param[in] x1 - starting `x` coordinate  of bond
  * \param[in] y1 - starting `y` coordinate  of bond
  * \param[in] z1 - starting `z` coordinate  of bond
  * \param[in] x2 - ending `x` coordinate  of bond
  * \param[in] y2 - ending `y` coordinate  of bond
  * \param[in] z2 - ending `z` coordinate  of bond
  * \param[in] radius - bond radius, == partilce radius / 3.
  * \param[in] stacks - stacks to fill in the cylinder
  */
void 
draw_nn_bond(float x1, 
             float y1, 
             float z1, 
             float x2,
             float y2, 
             float z2, 
             float radius,
             int   stacks);



/** \brief Track and process keyboard signals
  * \param[in] ch - which character was entered
  * \param[in] x - unused
  * \param[in] y - unused
  */
void 
keyboard_event(unsigned char ch, 
               int           x, 
               int           y);

/** \brief Track and process mouse signals
  * \param[in] button - which button was activated \n 
  *                     \arg \a left = rotate \a right = display menu \a middle = zoom
  * \param[in] state - state of the button, `1` = pressed , `0` released
  * \param[in] x - current x coordinate
  * \param[in] y - current y coordinate
  */
void 
mouse_clicked(int button, 
              int state, 
              int x, 
              int y);

/** \brief Track and process orientation of viewports from mouse/motion events
  * \param[in] x - current x coordinate
  * \param[in] y - current y coordinate
  */
void 
mouse_moved(int x, 
            int y);



/** \brief Track and process special keyboard signals, arrows, alt, ctrl, etc. being pressed
  * \param[in] key - which special key is pressed, uses GLUT_KEY_XXXX
  * \param[in] xx - unused
  * \param[in] yy - unused
  */
void
special_key_pressed(int key, 
                    int xx, 
                    int yy) ;

/** \brief Track and process special keyboard signals, arrows, alt, ctrl, etc. being released
  * \param[in] key - which special key was released, uses GLUT_KEY_XXXX
  * \param[in] x - unused
  * \param[in] y - unused
  */
void
special_key_release(int key, 
                    int x, 
                    int y) ;


void 
GetOGLPos(int x, int y);

/** \brief Load an array with rgb values for coloring different atomic species.\n
  * The coloring scheme used is CPK, which is the same as in Jmol
  */
void
build_colorTable();

#endif
