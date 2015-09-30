#include "tetragonal.h"

/*-----------------------------------------------------------------------
 *  Constructor called from menu for a lattice vector only
 *----------------------------------------------------------------------- */

Tetragonal::Tetragonal(double i_alat, double i_clat, char i_lstyle[], char *i_element) : LATTICE()
{
    species.push_back(i_element);
    alat = i_alat;
    blat = i_alat;
    clat = i_clat;

    alpha = beta = gamma = 90.0;
   	
    if (strncmp(i_lstyle,"SC",2)==0) {
        primitive();
    } else if (strncmp(i_lstyle,"BCC",3)==0) {
        BCC();
    } else if (strncmp(i_lstyle,"FCC",3)==0) {
        FCC();
    } else {
        printf("ERROR: Unknown lattice style [ %s ]!\n",i_lstyle);
    }
}

Tetragonal::~Tetragonal()
{
    
}

/* ----------------------------------------------------------------------
   Initialize for BCC lattice
------------------------------------------------------------------------- */
void Tetragonal::BCC()
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
void Tetragonal::FCC()
{
    char str[MAX_STRING_LENGTH];

    apcell = 4;
    ntype  = 1;
    basis = new basis_t[apcell];
 
    name = memory->new_1d<char>(4);
    strcpy(name, "FCC");

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
void Tetragonal::primitive()
{
    char str[MAX_STRING_LENGTH];

    apcell = 1;
    ntype  = 1;
    basis = new basis_t[apcell];

    name = memory->new_1d<char>(7);
    strcpy(name, "simple");

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