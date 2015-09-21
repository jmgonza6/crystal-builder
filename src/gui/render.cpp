#include "render.h"
//
// v 3.5.0
// supporting:
// full rotation
// full zoom ability
// perspective/orthographic projections
// particles radius increase/decrease
// coloring by atomic element 
// smooth shading and ambient lighting
//

float   pradius = 0.3;                   // starting particle radius
float   MINRADIUS = 0.05f;               // minimum radius for particles, prevents using a negative radius
float   MINSCALE = 0.05f;                // minimum allowable scale factor:
float   Scale = 2., Scale2 = 0.;         // scale factors for the scene
float   TransXYZ[3];                     // translation vectors for moving the object
float   deltaMove = 0;                   // to move the camera or not for translation
float   stheta = 0;                      // rotation about x axis 
float   sphi = 0;                        // rotation about y axis
float   sgamma = 0;                  // rotation about z axis
float   aspect = 4./3.;                  // aspect ratio of the window
float   segments = 80;
float   cutoff = 1.6;


float   Xp = 1.0f, Yp = 1.0f, Zp = 1.0f;

GLfloat ambientColor[] = 
		{ 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat light0Color[] = 
		{ 0.3f, 0.3f, 0.3f, 1.0f };


GLfloat light0Position[] =               // position of light 0, +x,+y,+z
		{ Xp, Yp, Zp, 0.0f};
GLfloat light1Position[] =               // position of light 1, +x,+y,-z
		{ Xp, Yp, -Zp, 0.0f};
GLfloat light2Position[] =               // position of light 2, -x,+y, -z
		{ -Xp, Yp, -Zp, 0.0f};
GLfloat light3Position[] =               // position of light 3, -x,+y,+z
		{ -Xp, Yp, Zp, 0.0f};

GLfloat light4Position[] =               // position of light 4, +x,-y,+z
		{ Xp, -Yp, Zp, 0.0f};
GLfloat light5Position[] =               // position of light 5, +x,-y,-z
		{ Xp, -Yp, -Zp, 0.0f};
GLfloat light6Position[] =               // position of light 6, -x,-y,-z
		{ -Xp, -Yp, -Zp, 0.0f};
GLfloat light7Position[] =               // position of light 7, -x,-y,+z
		{ -Xp, -Yp, Zp, 0.0f};



float   BACK[3];
float   colorList[110][3];                 // list of r,g,b values for coloring different atomic species
float   eyePosX = 0., eyePosY = 0., eyePosZ = 100.;

float   objCenter[3];

long    xsize;                           // width of the window after reshaping
long    ysize;                           // height of the window after reshaping

int     pincrease = 0, pdecrease =0;     // user increased/decreased particle radius
int     orthoProjection = 1;             // Ortho or Persp. view,  ortho == 1, persp == 0
int     ActiveButton;                    // current button that is pressed
int     LeftButton;                      // action for the mouse button, ROTATE or SCALE
int     Xmouse, Ymouse;                  // mouse position values
int     updates = 0;                     // how many times the scene has been redrawn, used in debugging

GLuint	UnitCellList;			         // display list id for the unit cell
GLuint  AtomsList;                       // display list id for the atomic data
GLuint  AxesList;                        // display list id for the coordinate axes
GLuint  BondList;                        // display list id for bonds between atoms

int     displayBonds = 0;
int     displayMenu;                     // secondary diaply options menu, 1 == show, 0 == hide
int     mainMenu;                        // id for the main rendering window
                                         // active mouse buttons (or them together)
int     LEFT   = 4;                      // define left button as 4
int     MIDDLE = 2;                      // define middle button as 2
int     RIGHT  = 1;                      // define right button as 1
int     displayMode = SHOWALL;           // what to show in the main rendering window, SHOWALL or ATOMSONLY
int     whiteBack = 0;

                                         // external data from the gui session
// float   **atoms;
int     ntotal;                          // how many atoms in the system
int     *typeList;

atom_t *atoms;

std::vector<std::vector<int> > nnlist;

Line    cellVectors[3];                  // lattice vectors a1 = 0, a2 = 1, a3 = 2


GLUquadricObj* myReusableQuadric = 0;


Errors *errors;

void 
gl_init() 
{
	errors = new Errors();

	#ifdef BLACKBACK
		BACK[0] = 0.0f;
		BACK[1] = 0.0f;
		BACK[2] = 0.0f;
	#else
		BACK[0] = 1.0f;
		BACK[1] = 1.0f;
		BACK[2] = 1.0f;
	#endif

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(WWIDTH, WHEIGHT);

	glutCreateWindow("Crystal Viewer: Ortho");

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);

	glDisable(GL_CULL_FACE);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glEnable (GL_POLYGON_SMOOTH);

	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glClearColor(BACK[0], BACK[1], BACK[2], 0.);

	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);

	glEnable (GL_LIGHTING);



	glEnable (GL_LIGHT0);
	glEnable (GL_LIGHT1);
	glEnable (GL_LIGHT2);
	glEnable (GL_LIGHT3);

	glEnable (GL_LIGHT4);
	glEnable (GL_LIGHT5);
	glEnable (GL_LIGHT6);
	glEnable (GL_LIGHT7);

	glEnable (GL_NORMALIZE);

	glShadeModel( GL_SMOOTH );


	glutReshapeFunc(reshape_window);

	glutDisplayFunc(render_data);

	glutKeyboardFunc(keyboard_event);

	glutMouseFunc(mouse_clicked);

	glutMotionFunc(mouse_moved); 

	// glutIdleFunc(update); // incremental update 

	glutSpecialFunc(keyboard_special_press); // process special key pressed
						// Warning: Nonstandard function! Delete if desired.
	glutSpecialUpFunc(keyboard_special_release); // process special key release
}

 

void 
gl_menu_init() 
{
	displayMenu = glutCreateMenu(set_display_type);

	glutAddMenuEntry("Show all (atoms + cell + axes)", SHOWALL);

	glutAddMenuEntry("Atoms only", ATOMSONLY);

	mainMenu = glutCreateMenu(kill_main_window);

	glutAddSubMenu("Display Options", displayMenu);

	glutAddMenuEntry("Exit", 99);

	glutAttachMenu(GLUT_RIGHT_BUTTON); 
}

void 
load_color_list()
{
	// type1
	colorList[0][0] = 230./255.; colorList[0][1] = 10./255.; colorList[0][2] = 10./255.;

	// type2
	colorList[1][0] = 247./255.; colorList[1][1] = 163./255.; colorList[1][2] = 16./255.;

	// type3
	colorList[2][0] = 27./255.; colorList[2][1] = 73./255.; colorList[2][2] = 255./255.;

	// type4
	colorList[3][0] = 1.0; colorList[3][1] = 1.0; colorList[3][2] = 1.0;

	// type5
	colorList[4][0] = 0.0; colorList[4][1] = 1.0; colorList[4][2] = 1.0;

	// type6
	colorList[5][0] = 1.0; colorList[5][1] = 1.0; colorList[5][2] = 0.5;

	// type7
	colorList[6][0] = 1.0; colorList[6][1] = 0.0; colorList[6][2] = 1.0;

	// type8
	colorList[7][0] = 1.0; colorList[7][1] = .5; colorList[7][2] = 1.0;

	// type 9
	colorList[8][0] = 1.0; colorList[8][1] = 1.0; colorList[8][2] = 0.0;
}



int
render_init(char outname[],  
	 		int natom, 
	 		int ntype, 
	 		std::vector<std::vector<double> > latvec, 
	 		int *atom_ids, 
	 		double **coords, 
	 		std::vector<std::string> species)
{

	load_color_list();

	TransXYZ[0] = TransXYZ[1] = TransXYZ[2] = 0.;

	ntotal = natom;

	// lattice vector a
	cellVectors[0].v[0].x[0] = 0.; cellVectors[0].v[0].x[1] = 0.; cellVectors[0].v[0].x[2] = 0.;
	cellVectors[0].v[1].x[0] = latvec[0][0]; cellVectors[0].v[1].x[1] = latvec[0][1]; cellVectors[0].v[1].x[2] = latvec[0][2];

	//lattice vector b
	cellVectors[1].v[0].x[0] = 0.; cellVectors[1].v[0].x[1] = 0.; cellVectors[1].v[0].x[2] = 0.;
	cellVectors[1].v[1].x[0] = latvec[1][0]; cellVectors[1].v[1].x[1] = latvec[1][1]; cellVectors[1].v[1].x[2] = latvec[1][2];

	// lattice vector c
	cellVectors[2].v[0].x[0] = 0.; cellVectors[2].v[0].x[1] = 0.; cellVectors[2].v[0].x[2] = 0.;
	cellVectors[2].v[1].x[0] = latvec[2][0]; cellVectors[2].v[1].x[1] = latvec[2][1]; cellVectors[2].v[1].x[2] = latvec[2][2];

	typeList = new int[natom];

	atoms = new atom_t[natom];

	// center of mass of the system
	double comx= 0, comy= 0, comz= 0;
	for (int n=0;n<natom;n++) {
		atoms[n].x = coords[n][0];
		atoms[n].y = coords[n][1];
		atoms[n].z = coords[n][2];
		typeList[n] = atom_ids[n];

		comx += atoms[n].x;
		comy += atoms[n].y;
		comz += atoms[n].z;
	}

	comx = comx / ntotal;
	comy = comy / ntotal;
	comz = comz / ntotal;

	objCenter[0] = comx;
	objCenter[1] = comy;
	objCenter[2] = comz;


	// calculate the initial nearest neighbor table based on the default
	// cutoff
	build_nn_list();

	// load the data into a call list for fast real time rendering
	if(!init_call_lists()) {
		errors->kill(FERR,"RENDER::render_init()", LERR, "Could not initialize the call lists!");
	}


	if (DMSG) {
		fprintf (FDEBUG, "RENDER::DEBUG::STATUS Object center:  %f, %f, %f\n",objCenter[0],objCenter[1],objCenter[2]);
		// for (int n=0;n<natom;n++) {
		// 	printf ("%f %f %f \n",atoms[n].x,atoms[n].y,atoms[n].z);
		// }
	}
		
	
}

int
build_nn_list()
{
    double x1,y1,z1;
    double x2,y2,z2;

    int nmax = 0;

    for (int i=0; i<ntotal; i++) {
    	if (!atoms[i].naborIds.empty()) atoms[i].naborIds.clear();
        x1 = atoms[i].x;
        y1 = atoms[i].y;
        z1 = atoms[i].z;
        for (int j=0; j<ntotal; j++) {
            x2 = atoms[j].x;
            y2 = atoms[j].y;
            z2 = atoms[j].z;
            double d = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1) );

            if (d <= cutoff && d !=0) {
                atoms[i].naborIds.push_back(j);
            }
        }
        nmax = nmax < atoms[i].naborIds.size() ? atoms[i].naborIds.size() : nmax;
    }

    return 1;
}

int
init_call_lists()
{
	float r,g,b;
	float lwidth = 1.0f;

	#ifdef BLACKBACK
		r = g = b = 1;
	#else
		r = g = b = 0;
	#endif

	// store the unit cell
	UnitCellList = glGenLists( 1 );
	glNewList( UnitCellList, GL_COMPILE );
		glDisable( GL_LIGHTING );
		float newo[3], newa[3], newb[3], newc[3];
		// start at the origin
		glTranslatef(0,0,0);
		glLineWidth(lwidth);
		// draw a
		glColor3f(r, g, b);
		glBegin(GL_LINE_STRIP);
			glVertex3fv( cellVectors[0].v[0].x );
			glVertex3fv( cellVectors[0].v[1].x );
		glEnd();
		// draw b
		glColor3f(r, g, b);
		glBegin(GL_LINE_STRIP);
			glVertex3fv( cellVectors[1].v[0].x );
			glVertex3fv( cellVectors[1].v[1].x );
		glEnd();
		// draw c
		glColor3f(r, g, b);
		glBegin(GL_LINE_STRIP);
			glVertex3fv( cellVectors[2].v[0].x );
			glVertex3fv( cellVectors[2].v[1].x );
		glEnd();
		// draw top a
		glColor3f(r, g, b);
		glBegin(GL_LINE_STRIP);
			newa[0] = cellVectors[0].v[1].x[0] + cellVectors[2].v[1].x[0];
			newa[1] = cellVectors[0].v[1].x[1] + cellVectors[2].v[1].x[1];
			newa[2] = cellVectors[2].v[1].x[2];
			glVertex3fv( cellVectors[2].v[1].x );
			glVertex3fv( newa );
		glEnd();
		// draw top b
		glColor3f(r, g, b);
		glBegin(GL_LINE_STRIP);
			newb[0] = cellVectors[1].v[1].x[0] + cellVectors[2].v[1].x[0];
			newb[1] = cellVectors[1].v[1].x[1] + cellVectors[2].v[1].x[1];
			newb[2] = cellVectors[2].v[1].x[2];
			glVertex3fv( cellVectors[2].v[1].x );
			glVertex3fv( newb );
		glEnd();
		// draw bottom b'
		glColor3f(r, g, b);
		glBegin(GL_LINE_STRIP);
			newb[0] = cellVectors[1].v[1].x[0] + cellVectors[0].v[1].x[0];
			newb[1] = cellVectors[1].v[1].x[1];
			newb[2] = cellVectors[1].v[0].x[2];
			glVertex3fv( cellVectors[0].v[1].x );
			glVertex3fv( newb );
		glEnd();
		// draw bottom a'
		glColor3f(r, g, b);
		glBegin(GL_LINE_STRIP);
			newa[0] = cellVectors[1].v[1].x[0] + cellVectors[0].v[1].x[0];
			newa[1] = cellVectors[1].v[1].x[1];
			newa[2] = cellVectors[1].v[0].x[2];
			glVertex3fv( cellVectors[1].v[1].x );
			glVertex3fv( newa );
		glEnd();
		// draw top a'
		glColor3f(r, g, b);
		glBegin(GL_LINE_STRIP);
			newo[0] = cellVectors[1].v[1].x[0] + cellVectors[2].v[1].x[0];
			newo[1] = cellVectors[1].v[1].x[1] + cellVectors[2].v[1].x[1];
			newo[2] = cellVectors[2].v[1].x[2];

			newa[0] = newo[0] + cellVectors[0].v[1].x[0];
			newa[1] = cellVectors[1].v[1].x[1] + cellVectors[2].v[1].x[1];
			newa[2] = cellVectors[2].v[1].x[2];
			glVertex3fv( newo );
			glVertex3fv( newa );
		glEnd();
		// draw top b'
		glColor3f(r, g, b);
		glBegin(GL_LINE_STRIP);
			newo[0] = cellVectors[0].v[1].x[0] + cellVectors[2].v[1].x[0];
			newo[1] = cellVectors[0].v[1].x[1] + cellVectors[2].v[1].x[1];
			newo[2] = cellVectors[2].v[1].x[2];

			newb[0] = cellVectors[1].v[1].x[0] + cellVectors[0].v[1].x[0] + cellVectors[2].v[1].x[0];
			newb[1] = cellVectors[1].v[1].x[1] + cellVectors[2].v[1].x[1];
			newb[2] = cellVectors[2].v[1].x[2];
			glVertex3fv( newo );
			glVertex3fv( newb );
		glEnd();
		// draw c'
		glColor3f(r, g, b);
		glBegin(GL_LINE_STRIP);
			// top
			newo[0] = cellVectors[1].v[1].x[0] + cellVectors[0].v[1].x[0] + cellVectors[2].v[1].x[0];
			newo[1] = cellVectors[1].v[1].x[1] + cellVectors[2].v[1].x[1];
			newo[2] = cellVectors[2].v[1].x[2];
			// bottom
			newc[0] = cellVectors[1].v[1].x[0] + cellVectors[0].v[1].x[0];
			newc[1] = cellVectors[1].v[1].x[1];
			newc[2] = cellVectors[1].v[1].x[2];
			glVertex3fv( newo );
			glVertex3fv( newc );
		glEnd();
		// draw top ab'
		glColor3f(r, g, b);
		glBegin(GL_LINE_STRIP);
			//top
			newo[0] = cellVectors[0].v[1].x[0] + cellVectors[2].v[1].x[0];
			newo[1] = cellVectors[0].v[1].x[1] + cellVectors[2].v[1].x[1];
			newo[2] = cellVectors[2].v[1].x[2];
			//bottom
			newa[0] = cellVectors[0].v[1].x[0];
			newa[1] = cellVectors[0].v[1].x[1];
			newa[2] = cellVectors[0].v[1].x[2];
			glVertex3fv( newo );
			glVertex3fv( newa );
		glEnd();
		// draw top ba'
		glColor3f(r, g, b);
		glBegin(GL_LINE_STRIP);
			// top
			newo[0] = cellVectors[1].v[1].x[0] + cellVectors[2].v[1].x[0];
			newo[1] = cellVectors[1].v[1].x[1] + cellVectors[2].v[1].x[1];
			newo[2] = cellVectors[2].v[1].x[2];
			// bottom
			newb[0] = cellVectors[1].v[1].x[0];
			newb[1] = cellVectors[1].v[1].x[1];
			newb[2] = cellVectors[1].v[1].x[2];
			glVertex3fv( newo );
			glVertex3fv( newb );
		glEnd();
		glEnable( GL_LIGHTING );
	glEndList( );


	// create the coordinate axes
	AxesList = glGenLists(1);
	glNewList(AxesList, GL_COMPILE);
		glDisable( GL_LIGHTING );
		float xyz[3];
		glLineWidth(4.0f);
		glColor3f(1.0,0.,0.);
		glBegin(GL_LINE_STRIP);
		xyz[0] = xyz[1] = xyz[2] =0; 
		glVertex3fv( xyz );
		xyz[0] = 3;
		glVertex3fv( xyz );
		glEnd();
		// draw y
		glColor3f(0.,1.,0.);
		glBegin(GL_LINE_STRIP);
		xyz[0] = xyz[1] = xyz[2] =0; 
		glVertex3fv( xyz );
		xyz[1] = 3;
		glVertex3fv( xyz );
		glEnd();
		// draw z
		glColor3f(0.,0.,1.);
		glBegin(GL_LINE_STRIP);
		xyz[0] = xyz[1] = xyz[2] =0;
		glVertex3fv( xyz );
		xyz[2] = 3;
		glVertex3fv( xyz );
		glEnd();
		glEnable( GL_LIGHTING );
	glEndList();

	// create the atoms
	build_nn_list();
	AtomsList = glGenLists(1);
	glNewList(AtomsList,GL_COMPILE);
		for (int n=0;n<ntotal;n++) {
			glColor3fv( colorList[typeList[n]-1] );
			glTranslatef( atoms[n].x , atoms[n].y, atoms[n].z );
	        glutSolidSphere( pradius, segments, segments );
	        glTranslatef( -atoms[n].x , -atoms[n].y, -atoms[n].z );
		}
	glEndList();

	return 1;
}

void 
draw_nn_bond(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions)
{
   //the same quadric can be re-used for drawing many cylinders
   GLUquadricObj *quadric=gluNewQuadric();
   gluQuadricNormals(quadric, GLU_SMOOTH);
		glColor3f(0.3,0.3,0.3);
		float vx = x2-x1;
		float vy = y2-y1;
		float vz = z2-z1;

		//handle the degenerate case of z1 == z2 with an approximation
		if(vz == 0)
		   vz = .00000001;

		float v = sqrt( vx*vx + vy*vy + vz*vz );
		float ax = 57.2957795*acos( vz/v );
		if ( vz < 0.0 )
		   ax = -ax;
		float rx = -vy*vz;
		float ry = vx*vz;
		glPushMatrix();

		//draw the cylinder body
		glTranslatef( x1,y1,z1 );
		glRotatef(ax, rx, ry, 0.0);
		gluQuadricOrientation(quadric,GLU_OUTSIDE);
		gluCylinder(quadric, radius, radius, v, subdivisions, 1);

		//draw the first cap
		gluQuadricOrientation(quadric,GLU_INSIDE);
		gluDisk( quadric, 0.0, radius, subdivisions, 1);
		glTranslatef( 0,0,v );

		//draw the second cap
		gluQuadricOrientation(quadric,GLU_OUTSIDE);
		gluDisk( quadric, 0.0, radius, subdivisions, 1);
		glPopMatrix();

   gluDeleteQuadric(quadric);
}


void 
show_all(int vport) 
{
	if (displayBonds) {
		// compute the nearest neighbor table
		// to determine which bonds connect where when the cutoff has been changed
		build_nn_list();
		float radius = pradius/3.;

		for (int n=0;n<ntotal;n++) {
			int ns = atoms[n].naborIds.size();
			for(int nn=0;nn<atoms[n].naborIds.size();nn++) {
				int naborId = atoms[n].naborIds[nn];
				//render the bond
				float x1 = atoms[n].x;
				float y1 = atoms[n].y;
				float z1 = atoms[n].z;
				float x2 = atoms[naborId].x;
				float y2 = atoms[naborId].y;
				float z2 = atoms[naborId].z;
				draw_nn_bond(x1,y1,z1,x2,y2,z2,radius,segments);
			}
		}
	}
	glCallList(UnitCellList);
	glCallList(AxesList);
	glCallList(AtomsList);
}

 
void
atoms_only(int vport)
{
	if (displayBonds) {
		// compute the nearest neighbor table
		// to determine which bonds connect where when the cutoff has been changed
		build_nn_list();
		float radius = pradius/3.;

		for (int n=0;n<ntotal;n++) {
			int ns = atoms[n].naborIds.size();
			for(int nn=0;nn<atoms[n].naborIds.size();nn++) {
				int naborId = atoms[n].naborIds[nn];
				//render the bond
				float x1 = atoms[n].x;
				float y1 = atoms[n].y;
				float z1 = atoms[n].z;
				float x2 = atoms[naborId].x;
				float y2 = atoms[naborId].y;
				float z2 = atoms[naborId].z;
				draw_nn_bond(x1,y1,z1,x2,y2,z2,radius,segments);
			}
		}
	}
	glCallList(AxesList);
	glCallList(AtomsList);
}
 
void 
reshape_window(int width, int height) 
{
	xsize = width;

	ysize = height;

	aspect = (float)xsize/(float)ysize;

	glViewport(0, 0, xsize, ysize);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutPostRedisplay();
}

 

void 
set_display_type(int value) 
{
	displayMode = value;
	switch(value) {
		case SHOWALL: glShadeModel(GL_SMOOTH); glEnable(GL_LIGHTING); break;
		case ATOMSONLY: glShadeModel(GL_SMOOTH); glEnable(GL_LIGHTING); break;
		case BONDS: glShadeModel(GL_SMOOTH); glEnable(GL_LIGHTING); break;
	}
	glutPostRedisplay();
}


void 
kill_main_window(int value) 
{
	switch(value) {
		case 99: glutDestroyWindow(value); break;
	}
}


// draw the complete scene:
void
render_data( )
{
    updates +=1;

    if (DMSG) {
    	fprintf( FDEBUG, "RENDER::DEBUG::STATUS Display update (%i)\n",updates);
    }
   
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    // set up all the lighting in the room
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Color);
    glLightfv(GL_LIGHT0, GL_POSITION, light0Position);

    glLightfv(GL_LIGHT1, GL_DIFFUSE, light0Color);
    glLightfv(GL_LIGHT1, GL_POSITION, light1Position);

    glLightfv(GL_LIGHT2, GL_DIFFUSE, light0Color);
    glLightfv(GL_LIGHT2, GL_POSITION, light2Position);

    glLightfv(GL_LIGHT3, GL_DIFFUSE, light0Color);
    glLightfv(GL_LIGHT3, GL_POSITION, light3Position);


    glLightfv(GL_LIGHT4, GL_DIFFUSE, light0Color);
    glLightfv(GL_LIGHT4, GL_POSITION, light4Position);

    glLightfv(GL_LIGHT5, GL_DIFFUSE, light0Color);
    glLightfv(GL_LIGHT5, GL_POSITION, light5Position);

    glLightfv(GL_LIGHT6, GL_DIFFUSE, light0Color);
    glLightfv(GL_LIGHT6, GL_POSITION, light6Position);

    glLightfv(GL_LIGHT7, GL_DIFFUSE, light0Color);
    glLightfv(GL_LIGHT7, GL_POSITION, light7Position);


		// glViewport(0, 0, w, h);
		// glMatrixMode(GL_PROJECTION);
		// glLoadIdentity();
		// GLdouble left = (0 - pointOfInterestX) / zoomLevel + pointOfInterestX;
		// GLdouble right = (WindowW - pointOfInterestX) / zoomLevel + pointOfInterestX;
		// GLdouble bottom = (WindowH - pointOfInterestY) / zoomLevel + pointOfInterestY;
		// GLdouble top = (0 - pointOfInterestY) / zoomLevel + pointOfInterestY;
		// glOrtho(left, right, bottom, top, -1, 1);
		// glMatrixMode(GL_MODELVIEW);
		// glLoadIdentity();


    // set the viewport to a square centered in the window:
    GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
    GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
    GLsizei v = vx < vy ? vx : vy;          // minimum dimension
    GLint xl = ( vx - v ) / 3;
    GLint yb = ( vy - v ) / 3;
    glViewport( xl, yb,  v+5, v+5 );
    // set the viewing volume
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    if( orthoProjection )
        glOrtho( -(VPORT_SIZE/2), VPORT_SIZE+20,     -(VPORT_SIZE/2), VPORT_SIZE+20,     0.1, 1000. );
    else
        gluPerspective( 45., 1.,    0.1, 1000. );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    // set the eye position, look-at position to be the center of mass of the crystal
    gluLookAt( objCenter[0], objCenter[1], eyePosZ,     objCenter[0], objCenter[1], 0.,     0., 1., 0. );

	// uniformly scale the scene:
    glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );
    GLfloat scale2 = 1. + Scale2;
    if( scale2 < MINSCALE )
        scale2 = MINSCALE;
    glScalef( (GLfloat)scale2, (GLfloat)scale2, (GLfloat)scale2 );

	//allow free rotation in all 3 dimensions at all times
	glRotatef(stheta, 1.0, 0.0, 0.0);

	glRotatef(sphi,   0.0, 1.0, 0.0);

	glRotatef(sgamma,  0.0, 0.0, 1.0);

	// if we changed the particle size, we need to rebuild
	// the glCallList() since these are constructed as "static"
	// objects when built.
	if (pincrease) {init_call_lists();pincrease=0;}
	if (pdecrease) {init_call_lists();pdecrease=0;}

	// place the objects into the scene by using glCallList()
	if (displayMode == SHOWALL) show_all(1); 
	else if (displayMode == ATOMSONLY) atoms_only(1);


    // swap the double-buffered framebuffers:
    glutSwapBuffers( );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    // be sure the graphics buffer has been sent:
    // note: be sure to use glFlush( ) here, not glFinish( ) !
    glFlush( );
}


void 
keyboard_event(unsigned char ch, int x, int y) 
{
	switch (ch) 
	{
		case 'A':
			if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:    showing atoms, unit cell, and coordinate axes. \n");
			
			displayMode = SHOWALL;
			break;
		case 'a':
			if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:    showing atoms and coordinate axes only. \n");
			
			displayMode = ATOMSONLY;
			break;
		case 'B':
			if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   bonds enabled\n");
			
			displayBonds = 1;
			break;
		case 'b':
			if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   bonds disabled\n");
			
			displayBonds = 0;
			break;
		case 'C':
			cutoff = cutoff + 0.05;
			if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   increasing bond cut-off, cutoff = %f Å\n",cutoff);
			
			break;
		case 'c':
			cutoff = cutoff - 0.05;
			cutoff = cutoff < 0.3 ? 0.3 : cutoff;
			if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   decreasing bond cut-off, cutoff = %f Å\n",cutoff);
			
			break;
		case 'e': 
			if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   saving and exiting\n");
				fprintf (SMESG,"___________________________________________________________\n");
			
			exit(0);
			break; 
		case 'r':
			if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   rotation enabled\n");
			
            LeftButton = ROTATE;
            break;
		case 'z':
			if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   zooming enabled\n");
			
            LeftButton = SCALE;
            break;
        case 't':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   translation enabled\n");
			
        	LeftButton = TRANSLATE;
        	break;
        case 'o':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   orthographic projection\n");
			
        	orthoProjection = 1;
        	break;
        case 'p':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   perspective projection\n");
			
        	orthoProjection = 0;
        	break;
        case 'W':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   white background\n");
			
        	whiteBack = 1;
        	break;
        case 'w':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   black background\n");
			
        	whiteBack = 0;
        	break;
        case '0':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   reseting orientation to xy plane\n");
			
        	stheta = sphi = sgamma =0;
        	break;
        case '1':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   xz plane\n");
			
        	stheta = -90;
        	sphi = 0;
        	sgamma = 0;
        	break;
        case '2':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   yz plane\n");
			
        	stheta = -90;
        	sphi = 0;
        	sgamma = -90;
        	break;
        case '3':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   xyz plane\n");
			
        	stheta = -45;
        	sphi = 0;
        	sgamma = -45;
        	break;
        case '+':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   increasing particle radius\n");
			
        	pincrease = 1;
        	pradius = pradius + 0.05;
        	break;
        case '-':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   decreasing particle radius\n");
			
        	pdecrease = 1;
        	pradius = pradius - 0.05;
        	pradius = pradius < MINRADIUS ? MINRADIUS : pradius;
        	break;
	} 
	glutPostRedisplay(); 
}


// called when the mouse button transitions down or up:
void
mouse_clicked( int button, int state, int x, int y )
{
    int b = 0;          // LEFT, MIDDLE, or RIGHT

    #ifdef DEBUG
        fprintf( FDEBUG, "RENDER::DEBUG::STATUS MouseButton: %d, %d, %d, %d\n", button, state, x, y );
    #endif
    
    // get the proper button bit mask:
    if (button == GLUT_LEFT_BUTTON) {
    	b = LEFT;    
    	#ifdef DEBUG
        	fprintf( FDEBUG, "RENDER::DEBUG::STATUS GLUT_LEFT_BUTTON pressed\n");
	    #endif 
    } else if (button == GLUT_MIDDLE_BUTTON) {
    	b = MIDDLE;     
        #ifdef DEBUG
        	fprintf( FDEBUG, "RENDER::DEBUG::STATUS GLUT_MIDDLE_BUTTON pressed\n");
	    #endif 
    } else if (button == GLUT_RIGHT_BUTTON) {
    	b = RIGHT;     
        #ifdef DEBUG
        	fprintf( FDEBUG, "RENDER::DEBUG::STATUS GLUT_RIGHT_BUTTON pressed\n");
	    #endif 
    } else {
    	b=0;
    	#ifdef DEBUG
        	fprintf( FDEBUG, "RENDER::DEBUG::STATUS Unknown button [ %d ] pressed\n", button);
	    #endif 
    }

    // button down sets the bit, up clears the bit:
    if( state == GLUT_DOWN ) {
        Xmouse = x;
        Ymouse = y;
        ActiveButton |= b;      // set the proper bit
    } else {
        ActiveButton &= ~b;     // clear the proper bit
    }
}

// called when the mouse moves while a button is down:
void
mouse_moved( int x, int y )
{
    #ifdef DEBUG
        fprintf( FDEBUG, "RENDER::DEBUG::STATUS MouseMotion: %d, %d\n", x, y );
    #endif

    int dx = x - Xmouse;        // change in mouse coords
    int dy = y - Ymouse;

    int sgnx = 1;
    int sgny = 1;
    if (dx<0) sgnx=-1;
    if (dy<0) sgny=-1;

    if( ( ActiveButton & LEFT ) != 0 ) {
    	if (LeftButton == ROTATE) {
    		sphi+=(float)(x-Xmouse)/4.0;
        	stheta+=(float)(Ymouse-y)/4.0; // rotate
    	} else if (LeftButton == SCALE) {
    		Scale += (float)(Ymouse - y)/10.;
            Scale = Scale < MINSCALE ? MINSCALE : Scale;
    	}
    }

    if( ( ActiveButton & MIDDLE ) != 0 ) {
    	Scale += (float)(Ymouse - y)/10.;
        Scale = Scale < MINSCALE ? MINSCALE : Scale;
    }

    Xmouse = x;         // new current position
    Ymouse = y;

    glutPostRedisplay( );
}


void 
keyboard_special_press(int key, int xx, int yy)
{
	switch (key) {
		case GLUT_KEY_UP : deltaMove = 1.0; break;
		case GLUT_KEY_DOWN : deltaMove = -1.0; break;
	}
} 

void 
keyboard_special_release(int key, int x, int y) 
{
	switch (key) {
		case GLUT_KEY_UP : deltaMove = 0.0; break;
		case GLUT_KEY_DOWN : deltaMove = 0.0; break;
	}
} 
 

void 
update()
{
	if (deltaMove) { // update camera position
		TransXYZ[0] += deltaMove  * 0.1;
		TransXYZ[1] += deltaMove  * 0.1;
	}
	glutPostRedisplay(); // redisplay everything
}
