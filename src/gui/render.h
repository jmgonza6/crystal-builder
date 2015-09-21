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
  *\b User interation:
  *  =============
  * \verbatim
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
  *     'r' and LeftButton    rotate the crystal in all 3 dimensions
  *     'z' and LeftButton    zoom toward/away from the crystal
  *     '0'                   reset orientation of crystal to looking down z toward xy plane
  *     '1'                   view the xz "a-c" plane of the crystal
  *     '2'                   view the yz "b-c" plane of the crystal
  *     '3'                   rotate crystal about x by -45˚, and then -45˚ about z, 'psuedo' perspective view
  *     '+'                   increase the particle radius uniformly
  *     '-'                   decrease the particle radius uniformly
  *
  *     gl_init()             initializes the display mode and render environment
  *
  *     gl_menu_init()        initializes a pop-up menu for changing display
  *                           paramaters in realtime, 
  *                           (particle radius, atoms only, and atoms+cell)
  *
  *     render_init()         initializes the geometry data defined in the gui
  *                           session 
  * \endverbatim
  * \author Joseph M. Gonzalez
  *
  * \version 0.7
  *
  * \date Sep 13, 2015 19:16:20
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
#define WWIDTH 1024       //!< width of the rendering window
#define WHEIGHT 1024       //!< height of the rendering window
#define VPORT_SIZE 50     //!< size of the view port, VPORT_SIZExVPORT_SIZE


/** \struct Point
  * \brief x,y,z coordinates for a point in space
  */
struct Point {
    float x[3];  
    float n[3]; 
};

/** \struct Line
  * \brief x,y,z point defining two vertices, used for drawing the unit cell
  */
struct Line {   
    Point v[2];  
};

/** \struct atom_t
  * \brief x,y,z point defining the atomic coordinates, and a vector for each atoms nearest neighbors
  */
typedef struct {
  double x,y,z;
  std::vector<int> naborIds;
} atom_t;

enum LeftButton {    ROTATE,SCALE,TRANSLATE    };

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
  * \param[in] outname - deprecated
  * \param[in] natom - total number of atoms created by lattice->build()
  * \param[in] ntype - number of unique types created by setting basis
  * \param[in] latvec - 2D vector containing the cell vectors, in cartesian units
  * \param[in] atom_ids - 1D list of atomic type ids 
  * \param[in] coords - 2D pointer to array containing the atomic positions, in cartesian units
  * \param[in] species - 1D vector containing atomic species
  */
int
render_init(char                              outname[],  
			int                               natom, 
			int                               ntype, 
			std::vector<std::vector<double> > latvec, 
			int                               *atom_ids, 
			double                            **coords, 
			std::vector<std::string>          species);




//
// "Private" drawing functions
//

/** \brief Initialize the atomic and unit cell data from a GUI session
  * Draws the objects and stores the data in a glList.  These lists  make rendering \n
  * much more efficient with a simple call to glCallList(glList)
  * \returns 1 if successful
  */
int
init_call_lists();


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

/** \brief Set the display type and render the data in different orientations in 4 view ports
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
  * \param[in] pradius - bond radius, == partilce radius / 3.
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

/** \brief Track and process special keyboard signals, arrows, alt, tab, etc.
  * \param[in] key - which special key is pressed, uses GLUT_KEY_XXXX
  * \param[in] xx - unused
  * \param[in] yy - unused
  */
void 
keyboard_special_press(int key, 
                       int xx, 
                       int yy);


/** \brief Track and process special keyboard signals, arrows, alt, tab, etc.
  * \param[in] key - which special key was released, uses GLUT_KEY_XXXX
  * \param[in] x - unused
  * \param[in] y - unused
  */
void 
keyboard_special_release(int key, 
                         int x, 
                         int y);

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


// update camera position
void 
update();

#endif
