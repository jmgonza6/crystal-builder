#include "lattice.h"

/* ----------------------------------------------------------------------
   constructor does nothing
------------------------------------------------------------------------- */
LATTICE::LATTICE()
{
    atoms = NULL;
    atomsOut = NULL;
    basis = NULL;

    natom = apcell = 0;
    ntype = 1;
    nx = ny = nz = 1;

    lstyle = NULL;
    atom_type = NULL;
    basis = NULL;
    fformat = NULL;

    alat = blat = clat = 0;

    memory = new Memory();
}

/* ----------------------------------------------------------------------
   deconstructor destroy any allocated memory
------------------------------------------------------------------------- */
LATTICE::~LATTICE()
{
    a1.clear(); a2.clear(); a3.clear();
    delete memory;
}


/* ----------------------------------------------------------------------
   method to generate atomic configurations
------------------------------------------------------------------------- */
void 
LATTICE::build_crystal()
{
    if (species.size()>ntype) 
        fprintf(FDEBUG,"WARNING!! species > ntype, using first element from list provided\n");

    char str[MAX_STRING_LENGTH];

    natom = nx*ny*nz*apcell;
    if (natom < 1) exit(1);

    int n = 0;

    if (DMSG) fprintf(FDEBUG, "LATTICE::DEBUG   >>");
    for (n=0;n<ntype;n++) {
        if (DMSG) fprintf(FDEBUG, " %i",nx*ny*nz*elemCount[n] );
        typeCount.push_back(nx*ny*nz*elemCount[n]);
    }
    if (DMSG) fprintf(FDEBUG, "\n");


    atoms = new atom_t[natom];
    

    // reset the counter for the atoms
    n=0;
    
    // write in the order needed by VASP
    // type_1 type_2 .... type_n
    // build in xy plane first, then add layers
    int x=0,y=0,z=0,i=0,t=0;
    double b1,b2,b3;
    for (i=0; i<apcell; i++){
        t = basis[i].typeId;
        b1 = basis[i].x;
        b2 = basis[i].y;
        b3 = basis[i].z;
        for (z=0; z<nz; z++){
            for (y=0; y<ny; y++){
                for (x=0; x<nx; x++){
                    atoms[n].typeId = t;
                    atoms[n].x = b1 + x;
                    atoms[n].y = b2 + y;
                    atoms[n].z = b3 + z;
                    n++;
                }
            }
        }
    }



    // convert the fractional lattice vectors to Å
    a1[0] *= alat; a1[1] *= alat; a1[2] *= alat;
    a2[0] *= blat; a2[1] *= blat; a2[2] *= blat;
    a3[0] *= clat; a3[1] *= clat; a3[2] *= clat;
    
    // particle n position
    std::vector<double> P(3);
    // ith component of lattice vectors
    std::vector<double> X(3);
    // tranform from lattice coordinates to real space coordinates
    for (n=0; n<natom; n++){
        P[0] = atoms[n].x; 
        P[1] = atoms[n].y; 
        P[2] = atoms[n].z;

        X[0] = a1[0]; X[1] = a2[0]; X[2] = a3[0];
        atoms[n].x = MATHNS::vector_dot(X,P);

        X[0] = a1[1]; X[1] = a2[1]; X[2] = a3[1];
        atoms[n].y = MATHNS::vector_dot(X,P);

        X[0] = a1[2]; X[1] = a2[2]; X[2] = a3[2];
        atoms[n].z = MATHNS::vector_dot(X,P);
    }

    // extend the lattice vectors to their supercell dimensions
    a1[0] *= nx; a1[1] *= nx; a1[2] *= nx;
    a2[0] *= ny; a2[1] *= ny; a2[2] *= ny;
    a3[0] *= nz; a3[1] *= nz; a3[2] *= nz;

    if (GMSG) fprintf(SMESG,"LATTICE::EXE   >> atoms created successfully\n");
}

/* ----------------------------------------------------------------------
   Display lattice basic info
------------------------------------------------------------------------- */
// void 
// LATTICE::show_info()
// {
//     if (!initialized) return;

//     std::string CLASS;
//     switch (lclass) {
//         case 1: CLASS = "bcc"; break;
//         case 2: CLASS = "diamond"; break;
//         case 3: CLASS = "fcc"; break;
//         case 4: CLASS = "hcp"; break;
//         case 5: CLASS = "custom"; break;
//         default: CLASS = "hcp";  break;
//     }

//     printf("\n"); for (int i=0; i<75; i++) printf("=");
//     printf("\n"); for (int i=0; i<27; i++) printf("=");
//     printf(" Lattice Info "); for (int i=0; i<34; i++) printf("="); printf("\n");
//     for (int i=0; i<75; i++) printf("=");printf("\n");
//     printf("Lattice class.....................:  %s\n", CLASS.c_str());
//     printf("Lattice style.....................:  %s\n", lstyle);
//     printf("Lattice parameter.................:  %f\n", alat);
//     printf("Cell dimensions...................:  %i x %i x %i\n",nx,ny,nz);
//     printf("Total atom count..................:  %i\n", natom);
//     printf("Number of atom per unit cell......:  %d\n", apcell);
//     printf("Number of atom types per unit cell:  %d\n", ntype);
//     printf("Output file format................:  %s\n", fformat);
//     printf("---------------------------------------------------\n");
//     printf("\nLattice vectors:\n");
//     for (int i=0; i<3; i++){
//         for(int j=0; j<3; j++) printf("%10.8f ", lvectors[i][j]);
//         printf("\n");
//     }
//     printf("---------------------------------------------------\n");
//     printf("\nBasis (atom type && fractional coordinates):\n");
//     for (int i=0; i<apcell; i++){
//         printf("%d %10.8f %10.8f %10.8f\n", atom_type[i], basis[i][0], basis[i][1], basis[i][2]);
//     }
// }
atom_t* LATTICE::copy_data(atom_t * atomsI)
{
    atom_t *atomO = new atom_t[natom];
    atom_t *p;
    for(int n=0; n<natom; n++) {
        p = &atomsI[n];
        atomO[n].x = p->x;
        atomO[n].y = p->y;
        atomO[n].z = p->z;
        atomO[n].typeId = p->typeId;
    }
    return atomO;
}


int
LATTICE::convert_coordinates(int fractional)
{
    atomsOut = new atom_t[natom];

    if (fractional) {
        atomsOut = cart2fract(a1,a2,a3,atoms);
        return 1;
    } else {
        atomsOut = copy_data(atoms);
        return 1;
    }

}

atom_t *
LATTICE::cart2fract(std::vector<double> a1, std::vector<double> a2, std::vector<double> a3, atom_t *atomsI) 
{
    atom_t *atomsO = new atom_t[natom];

    std::vector<double> X(3), P(3);
    char message[256];
    const char *subroutine = "cart2fract()";
    double alpha, beta, gamma;

    double A, B, C, vol;
    double sa, sb, sg; /* sin(alpha) sin(beta) sin(gamma) */
    double ca, cb, cg; /* cos(alpha) cos(beta) cos(gamma) */
    double x, y, z;
   
    A = MATHNS::vector_mag(a1);
    B = MATHNS::vector_mag(a2);
    C = MATHNS::vector_mag(a3);

    alpha = MATHNS::angle(a2,a3);
    beta = MATHNS::angle(a1,a3);
    gamma = MATHNS::angle(a1,a2);

    sa = sin(alpha), sb = sin(beta), sg = sin(gamma);
    ca = cos(alpha), cb = cos(beta), cg = cos(gamma);

    /* Unit volume of a triclinic cell, cimensionsless */
    vol = sqrt( (1. - (ca*ca) - (cb*cb) - (cg*cg) + 2.*ca*cb*cg) );

    /* transformation matrix for fract to cart coordinates */
    double **T = memory->new_2d<double>(3,3);

    T[0][0] = 1./A; T[0][1] = -cg/(A*sg); T[0][2] = (ca*cg-cb)/(A*vol*sg);
    T[1][0] = 0.;   T[1][1] = 1./(B*sg);  T[1][2] = (cb*cg-ca)/(B*vol*sg);
    T[2][0] = 0.;   T[2][0] = 0.;         T[2][2] = 1./(C*vol);

    atom_t *atomi;
    for(int ii=0; ii<natom; ii++) {
        P.erase(P.begin(),P.end());
        atomi = &atomsI[ii];

        X[0] = atomi->x;
        X[1] = atomi->y;
        X[2] = atomi->z;

        P = MATHNS::mat_vec_mult(3,T,X);

        atomsO[ii].x = P[0];
        atomsO[ii].y = P[1];
        atomsO[ii].z = P[2];
        atomsO[ii].typeId = atomi->typeId;
    }
    memory->destroy(T);
    return atomsO;
}
