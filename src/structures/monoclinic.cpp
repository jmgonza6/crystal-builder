#include "monoclinic.h"
#include "../util/mathns.h"

/*
 *-----------------------------------------------------------------------
 *  Constructor called from menu for a lattice vector only
 *----------------------------------------------------------------------- 
 */

Monoclinic::Monoclinic(double i_alat, 
                       double i_blat, 
                       double i_clat,
                       double i_beta,
                       int    i_apcell,
                       int    i_ntypes, 
                       char   i_lstyle[], 
                       char*  i_element) : LATTICE()
{
    char str[MAX_STRING_LENGTH];
    alat = i_alat;
    blat = i_blat;
    clat = i_clat;
    alpha = gamma = 90;
    beta = i_beta;

    apcell = i_apcell;
    ntype = i_ntypes;

    memory->new_1d(name,9);
    sprintf(str,"Monoclinic, β = %f",beta);
    strcpy(name, str);

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.);
    a2.push_back(1.);
    a2.push_back(0.);

    // rotate a1 by β to obtain a3
    a3 = MATHNS::rotate_vector(a1, "y", beta);

    species.push_back("Li");
    species.push_back("N");

    memory->new_1d(nt,ntype);
    memory->new_2d(basis,apcell,3);
    memory->new_1d(atom_type,apcell);


    atom_type[0] = 1;
    basis[0][0] = 0.;
    basis[0][1] = 0.;
    basis[0][2] = 0.;

    atom_type[1] = 2;
    basis[1][0] = 0.10;
    basis[1][1] = 0.3;
    basis[1][2] = 0.62;

    atom_type[2] = 2;
    basis[2][0] = 0.41;
    basis[2][1] = 0.0;
    basis[2][2] = 0.62;

    atom_type[3] = 2;
    basis[3][0] = -0.17;
    basis[3][1] = 0.29;
    basis[3][2] = 0.219;

    // nt[0] = 1;
    // nt[1] = 3;

    elemCount.push_back(1);
    elemCount.push_back(3);
}

Monoclinic::~Monoclinic()
{
    
}
