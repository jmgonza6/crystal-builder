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
    // char str[MAX_STRING_LENGTH];
    // alat = i_alat;
    // blat = i_blat;
    // clat = i_clat;
    // alpha = gamma = 90;
    // beta = i_beta;

    // apcell = i_apcell;
    // ntype = i_ntypes;

    // basis = new basis_t[apcell];

    // name = memory->new_1d<char>(MAX_STRING_LENGTH);
    // sprintf(str,"Monoclinic, β = %f",beta);
    // strcpy(name, str);

    // a1.push_back(1.);
    // a1.push_back(0.);
    // a1.push_back(0.);

    // a2.push_back(0.);
    // a2.push_back(1.);
    // a2.push_back(0.);

    // // rotate a1 by β to obtain a3
    // a3 = MATHNS::rotate_vector(a1, "y", beta);

    // species.push_back("Li");
    // species.push_back("N");

  
}

Monoclinic::~Monoclinic()
{
    
}
