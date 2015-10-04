#include "render.h"
//
// v 3.7.0
// supporting:
// full rotation
// full zoom ability
// perspective/orthographic projections
// particles radius increase/decrease
// coloring by atomic element, CPK/JMol style
// smooth shading and ambient lighting
// bonds with nearest neighbors
// backround color changing
// translation of crystal via arrow keys
//
float   bradius = 0.3;

float   pradius = 0.2;                   // starting particle radius
float   MINRADIUS = 0.05f;               // minimum radius for particles, prevents using a negative radius
float   MINSCALE = 0.05f;                // minimum allowable scale factor:
float   Scale = 2., Scale2 = 0.;         // scale factors for the scene
float   TransXYZ[3];                     // translation vectors for moving the object
float   stheta = 0;                      // rotation about x axis 
float   sphi = 0;                        // rotation about y axis
float   sgamma = 0;                  // rotation about z axis
float   segments = 80;
float   cutoff = 1.6;

float   delX = 0.05;
float   delY = 0.05;


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
int*    colorList;                       // list of indicies for locations in colorTable based on the types in species
float   colorTable[110][3];              // array of rgb values corresponding to the CPK system used in Jmol
float   pradii[110];
float   eyePosZ = 200.;


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
int     backChange = 0;
                                         // external data from the gui session
int     ntotal;                          // how many atoms in the system

atom_t* atoms;

std::vector<std::vector<int> > nnlist;
std::vector<std::string> elementList;

Line    cellVectors[3];                  // lattice vectors a1 = 0, a2 = 1, a3 = 2


Errors *errors;
Memory *memory;

PeriodicTable renderPTable = 
    /* Elements list */ //std::string
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
     "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt",
    /* Atomic masses */                 
    {0.,
     1.0079, 4.0026, 6.941, 9.0122, 10.811, 12.011, 14.007, 15.999, 18.998, 20.180, 
     22.990, 24.305, 26.982, 28.086, 30.974, 32.065, 35.453, 39.948, 39.098, 40.078, 
     44.956, 47.867, 50.942, 51.996, 54.938, 55.845, 58.933, 58.693, 63.546, 65.390,
     69.723, 72.610, 74.992, 78.960, 79.904, 83.800, 85.468, 87.620, 88.906, 91.224,
     92.906, 95.940, 98.000, 101.070, 102.910, 106.420, 107.87, 112.41, 114.82, 118.71,
     121.76, 127.60, 126.90, 131.29, 132.91, 137.33, 138.91, 140.12, 140.91, 144.24,
     145.00, 150.36, 151.96, 157.25, 158.93, 162.50, 164.93, 167.26, 168.93, 173.04,
     174.97, 178.49, 180.95, 183.84, 186.21, 190.23, 192.22, 195.08, 196.97, 200.59,
     204.38, 207.20, 208.98, 209.00, 210.00, 222.00, 223.00, 226.00, 227.00, 232.04,
     231.04, 238.03, 237.00, 244.00, 243.00, 247.00, 247.00, 251.00, 252.00, 257.00,
     258.00, 259.00, 262.00, 261.00, 262.00, 266.00, 264.00, 269.00, 268.00
    }
    };

void 
gl_init() 
{
	errors = new Errors();

	#ifdef BLACKBACK
		BACK[0] = 0.0f;
		BACK[1] = 0.0f;
		BACK[2] = 0.0f;
		whiteBack = 0;
	#else
		BACK[0] = 1.0f;
		BACK[1] = 1.0f;
		BACK[2] = 1.0f;
		whiteBack = 1;
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

	glutSpecialFunc(special_key_pressed);

	glutSpecialUpFunc(special_key_release);
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

int
render_init(int                               natom, 
	 		std::vector<double>               a1,
	 		std::vector<double>               a2,
	 		std::vector<double>               a3, 
	 		atom_t*                           atomsIn,
	 		std::vector<std::string>          species,
	 		std::vector<int>                  elemCount)
{


	// pradii = memory->new_1d<float>(110);
	float minr,maxr;

	minr = 0.2;
	maxr = 0.8;
	float dx = (maxr-minr)/(float(110-1));
	for (int d=0;d<110;d++) {
		pradii[d] = minr + d * dx;
	}

	TransXYZ[0] = TransXYZ[1] = TransXYZ[2] = 0.;

	ntotal = natom;

	// lattice vector a
	cellVectors[0].v[0].x[0] = 0.; cellVectors[0].v[0].x[1] = 0.; cellVectors[0].v[0].x[2] = 0.;
	cellVectors[0].v[1].x[0] = a1[0]; cellVectors[0].v[1].x[1] = a1[1]; cellVectors[0].v[1].x[2] = a1[2];

	//lattice vector b
	cellVectors[1].v[0].x[0] = 0.; cellVectors[1].v[0].x[1] = 0.; cellVectors[1].v[0].x[2] = 0.;
	cellVectors[1].v[1].x[0] = a2[0]; cellVectors[1].v[1].x[1] = a2[1]; cellVectors[1].v[1].x[2] = a2[2];

	// lattice vector c
	cellVectors[2].v[0].x[0] = 0.; cellVectors[2].v[0].x[1] = 0.; cellVectors[2].v[0].x[2] = 0.;
	cellVectors[2].v[1].x[0] = a3[0]; cellVectors[2].v[1].x[1] = a3[1]; cellVectors[2].v[1].x[2] = a3[2];


	atoms = new atom_t[natom];

	colorList = memory->new_1d<int>(natom);

	int s=0;
	int t=0;
	int c=0;
	int idx=0;
	// scan the list of elements in the system and match them
	// with those in the periodic table.  Use the index of
	// the periodic table to choose a color from colorTable
	for (s=0;s<species.size();s++) {
		for(int ii=1;ii<110;++ii) {
			if (species[s] == renderPTable.elements[ii]) {
				idx = ii-1;
				break;
		    }
		}
		// tag each atom with a color index from colorTable
		for (t=0;t<elemCount[s];t++) {
			colorList[c] = idx;
			c++;
		}
	}

	if (DMSG)
		for (s=0;s<species.size();s++) {
			fprintf(FDEBUG,"elementList[%i] %s\n",s,species[s].c_str() );
		}

	
	// load the rgb values into the main colorTable
	build_colorTable();


	for (int n=0;n<natom;n++) {
		atoms[n].x = atomsIn[n].x;
		atoms[n].y = atomsIn[n].y;
		atoms[n].z = atomsIn[n].z;
		atoms[n].typeId = atomsIn[n].typeId;
	}



	// calculate the initial nearest neighbor table based on the default
	// cutoff
	build_nn_list();

	// load the data into a call list for fast real time rendering
	if(!init_call_lists(4)) {
		errors->kill(FERR,"RENDER::render_init()", LERR, "Could not initialize the call lists!");
	}


	if (DMSG) {
		for (int n=0;n<natom;n++) {
			printf ("%f %f %f \n",atoms[n].x,atoms[n].y,atoms[n].z);
		}
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
init_call_lists(int list)
{
	if (list == 1) {
		init_bbox_list();
		return 1;
	} else if (list == 2) {
		init_axes_list();
		return 1; 
	}
	else if (list == 3) {
		init_atoms_list();
		return 1;
	} else if (list == 4) {
		init_bbox_list();
		init_axes_list();
		init_atoms_list();
		return 1;
	}
	return 0;
}

void
init_axes_list()
{
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
}

void
init_bbox_list()
{
	float r,g,b;
	float lwidth = 1.0f;

	if (whiteBack)
		r = g = b = 0;
	else
		r = g = b = 1;

	// store the unit cell
	UnitCellList = glGenLists( 1 );
	glNewList( UnitCellList, GL_COMPILE );
		glDisable( GL_LIGHTING );
		float newo[3], newa[3], newb[3], newc[3];
		// start at the origin
		// glTranslatef(0,0,0);
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
}

void
init_atoms_list()
{
	// create the atoms
	float vradius;
	build_nn_list();
	AtomsList = glGenLists(1);
	glNewList(AtomsList,GL_COMPILE);
		for (int n=0;n<ntotal;n++) {
			// printf ("colorList[n] = %i\n",colorList[n]);
			// vradius =  renderPTable.amu[colorList[n]+1]/50;
			glColor3fv( colorTable[colorList[n]] );
			glTranslatef( atoms[n].x , atoms[n].y, atoms[n].z );
	        glutSolidSphere( pradii[colorList[n]], segments, segments );
	        glTranslatef( -atoms[n].x , -atoms[n].y, -atoms[n].z );
		}
	glEndList();
}

void 
draw_nn_bond(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions)
{
   //the same quadric can be re-used for drawing many cylinders
   GLUquadricObj *quadric=gluNewQuadric();
   gluQuadricNormals(quadric, GLU_SMOOTH);
		
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
				glColor3f(.2,.2,.2);
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
   	glClear(GL_COLOR_BUFFER_BIT);

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


    // set the viewport to fill the entire window
    // based on its current dimensions
    GLsizei w,h;
    w = glutGet( GLUT_WINDOW_WIDTH );
    h = glutGet( GLUT_WINDOW_HEIGHT )==0 ? 1 : glutGet( GLUT_WINDOW_HEIGHT );

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glTranslatef(TransXYZ[0], TransXYZ[1], 0);

	if( orthoProjection ) {
		if (w <= h)
		    glOrtho (-VPORT_SIZE, VPORT_SIZE, -VPORT_SIZE*h/w, VPORT_SIZE*h/w, 0.1, 1000);
		else
		    glOrtho (-VPORT_SIZE*w/h, VPORT_SIZE*w/h, -VPORT_SIZE, VPORT_SIZE, 0.1, 1000);
	} else {
		if (w <= h)
			gluPerspective( 60., h/w,    0.1, 1000. );
		else 
			gluPerspective( 60., w/h,    0.1, 1000. );
	}
			

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    // set the eye position, look-at position to be the center of mass of the crystal
    gluLookAt( VPORT_SIZE/2, VPORT_SIZE/2, eyePosZ,      VPORT_SIZE/2, VPORT_SIZE/2,0.,     0., 1., 0. );

	// uniformly scale the scene:
    glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );
    GLfloat scale2 = 1. + Scale2;
    if( scale2 < MINSCALE )
        scale2 = MINSCALE;
    glScalef( (GLfloat)scale2, (GLfloat)scale2, (GLfloat)scale2 );

	//allow free rotation in all 3 dimensions at all times
	glRotatef(-stheta, 1.0, 0.0, 0.0);

	glRotatef(sphi,   0.0, 1.0, 0.0);

	glRotatef(sgamma, 0.0, 0.0, 1.0);

	// if the background changed colors, redraw the unit cell
	// in the appropriate color
	if (backChange) {init_call_lists(1);backChange=0;}

	// if we changed the particle size, we need to rebuild
	// the glCallList() since these are constructed as "static"
	// objects when built.
	if (pincrease) {init_call_lists(3);pincrease=0;}
	if (pdecrease) {init_call_lists(3);pdecrease=0;}

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
	switch (ch) {
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
			glClearColor(1, 1, 1, 1.0f);
	        glutPostRedisplay();
	        backChange = 1;
        	whiteBack = 1;
        	break;
        case 'w':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   black background\n");
			glClearColor(0, 0, 0, 1.0f);
	        glutPostRedisplay();
	        backChange = 1;
        	whiteBack = 0;
        	break;
        case '0':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   reseting orientation to xy plane\n");
			TransXYZ[0] = TransXYZ[1] = 0.;
        	stheta = sphi = sgamma =0;
        	break;
        case '1':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   xz plane\n");
			TransXYZ[0] = TransXYZ[1] = 0.;
        	stheta = 90;
        	sphi = 0;
        	sgamma = 0;
        	break;
        case '2':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   yz plane\n");
			TransXYZ[0] = TransXYZ[1] = 0.;
        	stheta = 90;
        	sphi = 0;
        	sgamma = -90;
        	break;
        case '3':
        	if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   xyz plane\n");
			TransXYZ[0] = TransXYZ[1] = 0.;
        	stheta = 70;
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
    	// if ( LeftButton == DRAG) {
    	// 	printf ("ctrl was pushed, dragging the crystal\n"); 
    	// 	GetOGLPos(x,y);
    	// } else 
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
special_key_release(int key, int x, int y) 
{
	  switch( key )
	   {
	   case GLUT_KEY_LEFT: //left
	      TransXYZ[0] = TransXYZ[0];
	      break;
	   case GLUT_KEY_RIGHT:  // right
	      TransXYZ[0] = TransXYZ[0];
	      break;
	   case GLUT_KEY_UP:  // up
	      TransXYZ[1] = TransXYZ[1];
	      break;
	   case GLUT_KEY_DOWN:  // down
	      TransXYZ[1] = TransXYZ[1];
	      break;
	   }

   glutPostRedisplay();
} 


void 
special_key_pressed(int key, int x, int y) 
{
	  switch( key )
	   {
	   case GLUT_KEY_LEFT: //left
	      TransXYZ[0] = TransXYZ[0]-delX;
	      if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   translating left\n");
	      break;
	   case GLUT_KEY_RIGHT:  // right
	      TransXYZ[0] = TransXYZ[0]+delX;
	      if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   translating right,\n");
	      break;
	   case GLUT_KEY_UP:  // up
	      TransXYZ[1] = TransXYZ[1]+delY;
	      if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   translating up\n");
	      break;
	   case GLUT_KEY_DOWN:  // down
	      TransXYZ[1] = TransXYZ[1]-delY;
	      if (RMSG)
				fprintf (SMESG,"RENDER::MESGS::STATUS:   translating down\n");
	      break;
	   }
   glutPostRedisplay();
} 

void 
GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    printf("x = %i , y= %i , newx = %f , newy = %f \n",x,y,posX,posY );
 
    TransXYZ[0] = posX;
    TransXYZ[1] = posY;

    // return CVector3(posX, posY, posZ);
}

void build_colorTable()
{
	// H
	colorTable[0][0] = 255./255.; colorTable[0][1] = 255./255.; colorTable[0][2] = 255./255.;
	
	// He
	colorTable[1][0] = 217./255.; colorTable[1][1] = 255./255.; colorTable[1][2] = 255./255.;

	// Li
	colorTable[2][0] = 204./255.; colorTable[2][1] = 128./255.; colorTable[2][2] = 255./255.;

	// Be
	colorTable[3][0] = 194./255.; colorTable[3][1] = 255./255.; colorTable[3][2] = 0/255.;

	// B
	colorTable[4][0] = 255./255.; colorTable[4][1] = 181./255.; colorTable[4][2] = 181./255.;

	// C
	colorTable[5][0] = 144./255.; colorTable[5][1] = 144./255.; colorTable[5][2] = 144./255.;

	// N
	colorTable[6][0] = 48./255.; colorTable[6][1] = 80./255.; colorTable[6][2] = 248./255.;

	// O
	colorTable[7][0] = 255./255.; colorTable[7][1] = 13./255.; colorTable[7][2] = 13./255.;

	// F
	colorTable[8][0] = 144./255.; colorTable[8][1] = 224./255.; colorTable[8][2] = 80./255.;

	// Ne
	colorTable[9][0] = 179./255.; colorTable[9][1] = 227./255.; colorTable[9][2] = 245./255.;


	// Na
	colorTable[10][0] = 171./255.; colorTable[10][1] = 92./255.; colorTable[10][2] = 242./255.;
	
	// Mg
	colorTable[11][0] = 138./255.; colorTable[11][1] = 255./255.; colorTable[11][2] = 0./255.;

	// Al
	colorTable[12][0] = 191./255.; colorTable[12][1] = 166./255.; colorTable[12][2] = 166./255.;

	// Si
	colorTable[13][0] = 240./255.; colorTable[13][1] = 200./255.; colorTable[13][2] = 160./255.;

	// P
	colorTable[14][0] = 255./255.; colorTable[14][1] = 128./255.; colorTable[14][2] = 0./255.;

	// S
	colorTable[15][0] = 255./255.; colorTable[15][1] = 255./255.; colorTable[15][2] = 48./255.;

	// Cl
	colorTable[16][0] = 31./255.; colorTable[16][1] = 240./255.; colorTable[16][2] = 31./255.;

	// Ar
	colorTable[17][0] = 128./255.; colorTable[17][1] = 209./255.; colorTable[17][2] = 227./255.;

	// K
	colorTable[18][0] = 143./255.; colorTable[18][1] = 64./255.; colorTable[18][2] = 212./255.;

	// Ca
	colorTable[19][0] = 61./255.; colorTable[19][1] = 255./255.; colorTable[19][2] = 0./255.;


	// Sc
	colorTable[20][0] = 230./255.; colorTable[20][1] = 230./255.; colorTable[20][2] = 230./255.;
	
	// Ti
	colorTable[21][0] = 191./255.; colorTable[21][1] = 194./255.; colorTable[21][2] = 199./255.;

	// V
	colorTable[22][0] = 166./255.; colorTable[22][1] = 166./255.; colorTable[22][2] = 171./255.;

	// Cr
	colorTable[23][0] = 138./255.; colorTable[23][1] = 153./255.; colorTable[23][2] = 199./255.;

	// Mn
	colorTable[24][0] = 156./255.; colorTable[24][1] = 122./255.; colorTable[24][2] = 199./255.;

	// Fe
	colorTable[25][0] = 224./255.; colorTable[25][1] = 102./255.; colorTable[25][2] = 51./255.;

	// Co
	colorTable[26][0] = 240./255.; colorTable[26][1] = 144./255.; colorTable[26][2] = 160./255.;

	// Ni
	colorTable[27][0] = 80./255.; colorTable[27][1] = 208./255.; colorTable[27][2] = 80./255.;

	// Cu
	colorTable[28][0] = 200./255.; colorTable[28][1] = 128./255.; colorTable[28][2] = 51./255.;

	// Zn
	colorTable[29][0] = 125./255.; colorTable[29][1] = 128./255.; colorTable[29][2] = 176./255.;


	// Ga
	colorTable[30][0] = 194./255.; colorTable[30][1] = 143./255.; colorTable[30][2] = 143./255.;
	
	// Ge
	colorTable[31][0] = 102./255.; colorTable[31][1] = 143./255.; colorTable[31][2] = 143./255.;

	// As
	colorTable[32][0] = 189./255.; colorTable[32][1] = 128./255.; colorTable[32][2] = 227./255.;

	// Se
	colorTable[33][0] = 255./255.; colorTable[33][1] = 161./255.; colorTable[33][2] = 0./255.;

	// Br
	colorTable[34][0] = 166./255.; colorTable[34][1] = 41./255.; colorTable[34][2] = 41./255.;

	// Kr
	colorTable[35][0] = 92./255.; colorTable[35][1] = 184./255.; colorTable[35][2] = 209./255.;

	// Rb
	colorTable[36][0] = 112./255.; colorTable[36][1] = 46./255.; colorTable[36][2] = 176./255.;

	// Sr
	colorTable[37][0] = 0./255.; colorTable[37][1] = 255./255.; colorTable[37][2] = 0./255.;

	// Y
	colorTable[38][0] = 148./255.; colorTable[38][1] = 255./255.; colorTable[38][2] = 255./255.;

	// Zr
	colorTable[39][0] = 148./255.; colorTable[39][1] = 224./255.; colorTable[39][2] = 224./255.;


	// Nb
	colorTable[40][0] = 115./255.; colorTable[40][1] = 194./255.; colorTable[40][2] = 201./255.;
	
	// Mo
	colorTable[41][0] = 84./255.; colorTable[41][1] = 181./255.; colorTable[41][2] = 181./255.;

	// Tc
	colorTable[42][0] = 59./255.; colorTable[42][1] = 158./255.; colorTable[42][2] = 158./255.;

	// Ru
	colorTable[43][0] = 36./255.; colorTable[43][1] = 143./255.; colorTable[43][2] = 143./255.;

	// Rh
	colorTable[44][0] = 10./255.; colorTable[44][1] = 125./255.; colorTable[44][2] = 140./255.;

	// Pd
	colorTable[45][0] = 0./255.; colorTable[45][1] = 105./255.; colorTable[45][2] = 133./255.;

	// Ag
	colorTable[46][0] = 192./255.; colorTable[46][1] = 192./255.; colorTable[46][2] = 192./255.;

	// Cd
	colorTable[47][0] = 255./255.; colorTable[47][1] = 217./255.; colorTable[47][2] = 143./255.;

	// In
	colorTable[48][0] = 166./255.; colorTable[48][1] = 117./255.; colorTable[48][2] = 115./255.;

	// Sn
	colorTable[49][0] = 102./255.; colorTable[49][1] = 128./255.; colorTable[49][2] = 128./255.;


	// Sb
	colorTable[50][0] = 158./255.; colorTable[50][1] = 99./255.; colorTable[50][2] = 181./255.;
	
	// Te
	colorTable[51][0] = 212./255.; colorTable[51][1] = 122./255.; colorTable[51][2] = 0./255.;

	// I
	colorTable[52][0] = 148./255.; colorTable[52][1] = 0./255.; colorTable[52][2] = 148./255.;

	// Xe
	colorTable[53][0] = 66./255.; colorTable[53][1] = 158./255.; colorTable[53][2] = 176./255.;

	// Cs
	colorTable[54][0] = 87./255.; colorTable[54][1] = 23./255.; colorTable[54][2] = 143./255.;

	// Ba
	colorTable[55][0] = 0./255.; colorTable[55][1] = 201./255.; colorTable[55][2] = 0./255.;

	// La
	colorTable[56][0] = 112./255.; colorTable[56][1] = 212./255.; colorTable[56][2] = 255./255.;

	// Ce
	colorTable[57][0] = 255./255.; colorTable[57][1] = 255./255.; colorTable[57][2] = 199./255.;

	// Pr
	colorTable[58][0] = 217./255.; colorTable[58][1] = 255./255.; colorTable[58][2] = 199./255.;

	// Nd
	colorTable[59][0] = 199./255.; colorTable[59][1] = 255./255.; colorTable[59][2] = 199./255.;


	// Pm
	colorTable[60][0] = 163./255.; colorTable[60][1] = 255./255.; colorTable[60][2] = 199./255.;
	
	// Sm
	colorTable[61][0] = 143./255.; colorTable[61][1] = 255./255.; colorTable[61][2] = 199./255.;

	// Eu
	colorTable[62][0] = 97./255.; colorTable[62][1] = 255./255.; colorTable[62][2] = 199./255.;

	// Gd
	colorTable[63][0] = 69./255.; colorTable[63][1] = 255./255.; colorTable[63][2] = 199./255.;

	// Tb
	colorTable[64][0] = 48./255.; colorTable[64][1] = 255./255.; colorTable[64][2] = 199./255.;

	// Dy
	colorTable[65][0] = 31./255.; colorTable[65][1] = 255./255.; colorTable[65][2] = 199./255.;

	// Ho
	colorTable[66][0] = 0./255.; colorTable[66][1] = 255./255.; colorTable[66][2] = 156./255.;

	// Er
	colorTable[67][0] = 0./255.; colorTable[67][1] = 230./255.; colorTable[67][2] = 117./255.;

	// Tm
	colorTable[68][0] = 0./255.; colorTable[68][1] = 212./255.; colorTable[68][2] = 82./255.;

	// Yb
	colorTable[69][0] = 0./255.; colorTable[69][1] = 191./255.; colorTable[69][2] = 56./255.;


	// Lu
	colorTable[70][0] = 0./255.; colorTable[70][1] = 171./255.; colorTable[70][2] = 36./255.;
	
	// Hf
	colorTable[71][0] = 77./255.; colorTable[71][1] = 194./255.; colorTable[71][2] = 255./255.;

	// Ta
	colorTable[72][0] = 77./255.; colorTable[72][1] = 166./255.; colorTable[72][2] = 255./255.;

	// W
	colorTable[73][0] = 33./255.; colorTable[73][1] = 148./255.; colorTable[73][2] = 214./255.;

	// Re
	colorTable[74][0] = 38./255.; colorTable[74][1] = 125./255.; colorTable[74][2] = 171./255.;

	// Os
	colorTable[75][0] = 38./255.; colorTable[75][1] = 102./255.; colorTable[75][2] = 150./255.;

	// Ir
	colorTable[76][0] = 23./255.; colorTable[76][1] = 84./255.; colorTable[76][2] = 135./255.;

	// Pt
	colorTable[77][0] = 208./255.; colorTable[77][1] = 208./255.; colorTable[77][2] = 224./255.;

	// Au
	colorTable[78][0] = 255./255.; colorTable[78][1] = 209./255.; colorTable[78][2] = 35./255.;

	// Hg
	colorTable[79][0] = 184./255.; colorTable[79][1] = 184./255.; colorTable[79][2] = 208./255.;


	// Tl
	colorTable[80][0] = 166./255.; colorTable[80][1] = 84./255.; colorTable[80][2] = 77./255.;
	
	// Pb
	colorTable[81][0] = 87./255.; colorTable[81][1] = 89./255.; colorTable[81][2] = 97./255.;

	// Bi
	colorTable[82][0] = 158./255.; colorTable[82][1] = 79./255.; colorTable[82][2] = 181./255.;

	// Po
	colorTable[83][0] = 171./255.; colorTable[83][1] = 92./255.; colorTable[83][2] = 0./255.;

	// At
	colorTable[84][0] = 117./255.; colorTable[84][1] = 79./255.; colorTable[84][2] = 69./255.;

	// Rn
	colorTable[85][0] = 66./255.; colorTable[85][1] = 0./255.; colorTable[85][2] = 102./255.;

	// Fr
	colorTable[86][0] = 66./255.; colorTable[86][1] = 0./255.; colorTable[86][2] = 122./255.;

	// Ra
	colorTable[87][0] = 0./255.; colorTable[87][1] = 125./255.; colorTable[87][2] = 0./255.;

	// Ac
	colorTable[88][0] = 112./255.; colorTable[88][1] = 171./255.; colorTable[88][2] = 250./255.;

	// Th
	colorTable[89][0] = 0./255.; colorTable[89][1] = 186./255.; colorTable[89][2] = 255./255.;


	// Pa
	colorTable[90][0] = 0./255.; colorTable[90][1] = 161./255.; colorTable[90][2] = 255./255.;
	
	// U
	colorTable[91][0] = 0./255.; colorTable[91][1] = 143./255.; colorTable[91][2] = 255./255.;

	// Np
	colorTable[92][0] = 0./255.; colorTable[92][1] = 128./255.; colorTable[92][2] = 255./255.;

	// Pu
	colorTable[93][0] = 0./255.; colorTable[93][1] = 107./255.; colorTable[93][2] = 255./255.;

	// Am
	colorTable[94][0] = 84./255.; colorTable[94][1] = 92./255.; colorTable[94][2] = 227./255.;

	// Cm
	colorTable[95][0] = 120./255.; colorTable[95][1] = 92./255.; colorTable[95][2] = 227./255.;

	// Bk
	colorTable[96][0] = 138./255.; colorTable[96][1] = 79./255.; colorTable[96][2] = 227./255.;

	// Cf
	colorTable[97][0] = 161./255.; colorTable[97][1] = 54./255.; colorTable[97][2] = 212./255.;

	// Es
	colorTable[98][0] = 179./255.; colorTable[98][1] = 31./255.; colorTable[98][2] = 186./255.;

	// Fm
	colorTable[99][0] = 179./255.; colorTable[99][1] = 31./255.; colorTable[99][2] = 186./255.;


	// Md
	colorTable[100][0] = 179./255.; colorTable[100][1] = 13./255.; colorTable[100][2] = 166./255.;
	
	// No
	colorTable[101][0] = 189./255.; colorTable[101][1] = 13./255.; colorTable[101][2] = 135./255.;

	// Lr
	colorTable[102][0] = 199./255.; colorTable[102][1] = 0./255.; colorTable[102][2] = 102./255.;

	// Rf
	colorTable[103][0] = 204./255.; colorTable[103][1] = 0./255.; colorTable[103][2] = 89./255.;

	// Db
	colorTable[104][0] = 209./255.; colorTable[104][1] = 0./255.; colorTable[104][2] = 79./255.;

	// Sg
	colorTable[105][0] = 217./255.; colorTable[105][1] = 0./255.; colorTable[105][2] = 69./255.;

	// Bh
	colorTable[106][0] = 224./255.; colorTable[106][1] = 0./255.; colorTable[106][2] = 56./255.;

	// Hs
	colorTable[107][0] = 230./255.; colorTable[107][1] = 0./255.; colorTable[107][2] = 46./255.;

	// Mt
	colorTable[108][0] = 235./255.; colorTable[108][1] = 0./255.; colorTable[108][2] = 38./255.;
}
