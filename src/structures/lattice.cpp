#include "lattice.h"

/* ----------------------------------------------------------------------
   constructor does nothing
------------------------------------------------------------------------- */
LATTICE::LATTICE()
{
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

    memory->new_2d(coords,natom,3);
    memory->new_1d(type_list,natom);

    memory->new_1d(type_count,ntype);

    int n = 0;
    // write in the order needed by VASP
    // type_1 type_2 .... type_npcell
    // build in xy plane first, then add layers
    for (int u=0; u<apcell; u++){
        for (int k=0; k<nz; k++){
            for (int j=0; j<ny; j++){
                for (int i=0; i<nx; i++){
                    type_list[n] = atom_type[u];
                    coords[n][0] = basis[u][0] + double(i);
                    coords[n][1] = basis[u][1] + double(j);
                    coords[n][2] = basis[u][2] + double(k);
                    n++;
                }
            }
        }
    }




    if (DMSG) fprintf(FDEBUG, "LATTICE::DEBUG   >>");
    for (n=0;n<ntype;n++) {
        if (DMSG) fprintf(FDEBUG, " %i",nx*ny*nz*nt[n] );
        type_count[n] = nx*ny*nz*elemCount[n];
    }
    if (DMSG) fprintf(FDEBUG, "\n");

    // particle n position
    std::vector<double> P(3);
    // ith component of lattice vectors
    std::vector<double> X(3);
    for (int i=0; i<3; i++){
        a1[i] = a1[i]*alat;
        a2[i] = a2[i]*blat;
        a3[i] = a3[i]*clat;
    }

    // tranform lattice coordinates to real space coordinates
    for (n=0; n<natom; n++){
        P[0] = coords[n][0]; 
        P[1] = coords[n][1]; 
        P[2] = coords[n][2];
        for (int i=0;i<3;i++) {
            X[0] = a1[i]; X[1] = a2[i]; X[2] = a3[i];
            coords[n][i] = MATHNS::vector_dot(X,P);
        }
    }

    // extend the lattice vectors to their supercell dimensions
    for (int i=0; i<3; i++){
        a1[i] *= double(nx);
        a2[i] *= double(ny);
        a3[i] *= double(nz);
    }

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

int
LATTICE::convert_coordinates(int fractional)
{
    if (fractional) {
        cart2fract(a1,a2,a3,coords);
        return 1;
    } else {
        memory->new_2d(positions,natom,3);
        for(int ii=0; ii<natom; ii++) {
            positions[ii][0] = coords[ii][0];
            positions[ii][1] = coords[ii][1];
            positions[ii][2] = coords[ii][2];
        }
        return 1;
    }

}

void
LATTICE::cart2fract(std::vector<double> a1, std::vector<double> a2, std::vector<double> a3, double **cartesian) 
{
    memory->new_2d(positions,natom,3);

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
    double **T;
    memory->new_2d(T,3,3);

    T[0][0] = 1./A; T[0][1] = -cg/(A*sg); T[0][2] = (ca*cg-cb)/(A*vol*sg);
    T[1][0] = 0.;   T[1][1] = 1./(B*sg);  T[1][2] = (cb*cg-ca)/(B*vol*sg);
    T[2][0] = 0.;   T[2][0] = 0.;         T[2][2] = 1./(C*vol);


    for(int ii=0; ii<natom; ii++) {
        P.erase(P.begin(),P.end());
        X[0] = cartesian[ii][0]; // x
        X[1] = cartesian[ii][1]; // y
        X[2] = cartesian[ii][2]; // z
        P = MATHNS::mat_vec_mult(3,T,X);
        positions[ii][0] = P[0];
        positions[ii][1] = P[1];
        positions[ii][2] = P[2];
    }
    memory->destroy(T);
}

int
LATTICE::build_nn_list()
{
    // nn_search();
    return 1;
}

void
LATTICE::nn_search()
{
    
}