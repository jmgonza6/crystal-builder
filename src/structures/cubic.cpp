#include "cubic.h"

/*-----------------------------------------------------------------------
 *  Constructor called from menu for a lattice vector only
 *----------------------------------------------------------------------- */

Cubic::Cubic(double i_alat, char i_lstyle[], char* i_element) : LATTICE()
{
    alat = i_alat;
    blat = i_alat;
    clat = i_alat;
    species.push_back(i_element);

    alpha = beta = gamma = 90.0;
   	
    if (strncmp(i_lstyle,"SC",2)==0) {
        primitive();
    } else if (strncmp(i_lstyle,"BCC",3)==0) {
        BCC();
    } else if (strncmp(i_lstyle,"FCC",3)==0) {
        FCC();
    } else if (strncmp(i_lstyle,"Diamond",7)==0) {
        Diamond();
    } else {
        printf("ERROR: Unknown lattice style [ %s ]!\n",i_lstyle);
    }
}

Cubic::~Cubic()
{

}

/* ----------------------------------------------------------------------
   Initialize for BCC lattice
------------------------------------------------------------------------- */
void Cubic::BCC()
{
    char str[MAX_STRING_LENGTH];

    apcell = 2;
    ntype  = 1;

    basis = new basis_t[apcell];

    name = memory->new_1d<char>(4);
    strcpy(name, "BCC");


    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.);
    a2.push_back(1.);
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(1.);    


    basis[0].x = 0.;
    basis[0].y = 0.;
    basis[0].z = 0.;
    basis[0].typeId = 1;

    basis[1].x = 0.5;
    basis[1].y = 0.5;
    basis[1].z = 0.5;
    basis[1].typeId = 1;

    elemCount.push_back(2);

}

/* ----------------------------------------------------------------------
   Initialize for FCC lattice
------------------------------------------------------------------------- */
void Cubic::FCC()
{
    char str[MAX_STRING_LENGTH];
    name = memory->new_1d<char>(4);
    strcpy(name, "FCC");

    apcell = 4;
    ntype  = 1;
    basis = new basis_t[apcell];

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.);
    a2.push_back(1.);
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(1.);  

    basis[0].x = 0.;
    basis[0].y = 0.;
    basis[0].z = 0.;
    basis[0].typeId = 1;

    basis[1].x = 0.5;
    basis[1].y = 0.5;
    basis[1].z = 0.0;
    basis[1].typeId = 1;

    basis[2].x = 0.0;
    basis[2].y = 0.5;
    basis[2].z = 0.5;
    basis[2].typeId = 1;

    basis[2].x = 0.5;
    basis[2].y = 0.0;
    basis[2].z = 0.5;
    basis[2].typeId = 1;

    elemCount.push_back(4);
}

/* ----------------------------------------------------------------------
   Initialize for (001) orientation
------------------------------------------------------------------------- */
void Cubic::primitive()
{
    char str[MAX_STRING_LENGTH];
 
    name = memory->new_1d<char>(13);
    strcpy(name, "simple cubic");


    apcell = 1;
    ntype  = 1;

    basis = new basis_t[apcell];

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.);
    a2.push_back(1.);
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(1.);  

    basis[0].x = 0.;
    basis[0].y = 0.;
    basis[0].z = 0.;
    basis[0].typeId = 1;

    elemCount.push_back(1);
}

/* ----------------------------------------------------------------------
   Initialize for diamond along (001) orientation
------------------------------------------------------------------------- */
void Cubic::Diamond()
{
    name = memory->new_1d<char>(8);
    strcpy(name, "Diamond");

    apcell = 8;
    ntype  = 1;
    basis = new basis_t[apcell];
  
    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.);
    a2.push_back(1.);
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(1.);  
    
    basis[0].x = 0.;
    basis[0].y = 0.;
    basis[0].z = 0.;
    basis[0].typeId = 1;

    basis[1].x = 0.5;
    basis[1].y = 0.5;
    basis[1].z = 0.0;
    basis[1].typeId = 1;

    basis[2].x = 0.25;
    basis[2].y = 0.25;
    basis[2].z = 0.25;
    basis[2].typeId = 1;

    basis[2].x = 0.75;
    basis[2].y = 0.75;
    basis[2].z = 0.25;
    basis[2].typeId = 1;

    basis[3].x = 0.5;
    basis[3].y = 0.;
    basis[3].z = 0.5;
    basis[3].typeId = 1;

    basis[4].x = 0.0;
    basis[4].y = 0.5;
    basis[4].z = 0.5;
    basis[4].typeId = 1;

    basis[5].x = 0.75;
    basis[5].y = 0.25;
    basis[5].z = 0.75;
    basis[5].typeId = 1;

    basis[6].x = 0.25;
    basis[6].y = 0.75;
    basis[6].z = 0.75;
    basis[6].typeId = 1;

    elemCount.push_back(8);
}
