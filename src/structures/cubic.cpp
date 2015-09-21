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
    int surftype = 1;
    std::vector<double> row(3);

    memory->new_1d(name,9);
    strcpy(name, "BCC");

    apcell = 2;
    ntype  = 1;
    memory->new_1d(nt,ntype);

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.);
    a2.push_back(1.);
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(1.);    

    memory->new_2d(basis,apcell,3);
    memory->new_1d(atom_type,apcell);

    for (int i=0; i<apcell; i++) atom_type[i] = 1;
    basis[0][0] = 0.;
    basis[0][1] = 0.;
    basis[0][2] = 0.;

    basis[1][0] = 0.5;
    basis[1][1] = 0.5;
    basis[1][2] = 0.5;

    elemCount.push_back(2);
    // nt[0] = 2;
}

/* ----------------------------------------------------------------------
   Initialize for FCC lattice
------------------------------------------------------------------------- */
void Cubic::FCC()
{
    char str[MAX_STRING_LENGTH];
    int surftype = 1;
    std::vector<double> row(3);

    memory->new_1d(name,9);
    strcpy(name, "FCC");

    apcell = 4;
    ntype  = 1;
    memory->new_1d(nt,ntype);

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.);
    a2.push_back(1.);
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(1.);  
    
    memory->new_2d(basis,apcell,3);
    memory->new_1d(atom_type,apcell);

    for (int i=0; i<apcell; i++) atom_type[i] = 1;
    basis[0][0] = 0.;
    basis[0][1] = 0.;
    basis[0][2] = 0.;

    basis[1][0] = 0.5;
    basis[1][1] = 0.5;
    basis[1][2] = 0.;

    basis[2][0] = 0.;
    basis[2][1] = 0.5;
    basis[2][2] = 0.5;

    basis[3][0] = 0.5;
    basis[3][1] = 0.;
    basis[3][2] = 0.5;

    elemCount.push_back(4);
    // nt[0] = 4;
}

/* ----------------------------------------------------------------------
   Initialize for (001) orientation
------------------------------------------------------------------------- */
void Cubic::primitive()
{
    char str[MAX_STRING_LENGTH];
    int surftype = 1;
    std::vector<double> row(3);

    memory->new_1d(name,9);
    strcpy(name, "primitive");

    apcell = 1;
    ntype  = 1;

    memory->new_1d(nt,ntype);

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.);
    a2.push_back(1.);
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(1.);  

    memory->new_2d(basis,apcell,3);
    memory->new_1d(atom_type,apcell);

    for (int i=0; i<apcell; i++) atom_type[i] = 1;
    basis[0][0] = 0.;
    basis[0][1] = 0.;
    basis[0][2] = 0.;

    elemCount.push_back(1);
    // nt[0] = 1;
}

/* ----------------------------------------------------------------------
   Initialize for diamond along (001) orientation
------------------------------------------------------------------------- */
void Cubic::Diamond()
{
    std::vector<double> row(3);
    memory->new_1d(name,9);
    strcpy(name, "Diamond");

    apcell = 8;
    ntype  = 1;
    memory->new_1d(nt,ntype);
  
    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    a2.push_back(0.);
    a2.push_back(1.);
    a2.push_back(0.);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(1.);  

    memory->new_2d(basis,apcell,3);
    memory->new_1d(atom_type,apcell);
    
    for (int i=0; i<apcell; i++) atom_type[i] = 1;
    basis[0][0] = 0.;
    basis[0][1] = 0.;
    basis[0][2] = 0.;
  
    basis[1][0] = 0.5;
    basis[1][1] = 0.5;
    basis[1][2] = 0.;
  
    basis[2][0] = 0.25;
    basis[2][1] = 0.25;
    basis[2][2] = 0.25;
  
    basis[3][0] = 0.75;
    basis[3][1] = 0.75;
    basis[3][2] = 0.25;
  
    basis[4][0] = 0.5;
    basis[4][1] = 0.0;
    basis[4][2] = 0.5;
  
    basis[5][0] = 0.0;
    basis[5][1] = 0.5;
    basis[5][2] = 0.5;
  
    basis[6][0] = 0.75;
    basis[6][1] = 0.25;
    basis[6][2] = 0.75;
  
    basis[7][0] = 0.25;
    basis[7][1] = 0.75;
    basis[7][2] = 0.75; 

    elemCount.push_back(8);
    // nt[0] = 8;
}
