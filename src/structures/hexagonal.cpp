#include "hexagonal.h"

Hexagonal::Hexagonal(double i_alat, char*  i_element)
{
	alat = blat = clat = i_alat;
	species.push_back(i_element);
	primitive();
}

Hexagonal::~Hexagonal()
{

}

void
Hexagonal::primitive()
{
    memory->new_1d(name,9);
    strcpy(name, "hcp");
    apcell = 2;
    ntype  = 1;
    memory->new_1d(nt,ntype);
    nt[0] = 2;

    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    alpha = beta = 90.0;
    gamma = 120.0;

    a2 = MATHNS::rotate_vector(a1,"z",gamma);

    a3.push_back(0.);
    a3.push_back(0.);
    a3.push_back(sqrt(8./3.));

    memory->new_2d(basis,apcell,3);
    memory->new_1d(atom_type,apcell);

    for (int i=0; i<apcell; i++) atom_type[i] = 1;

    basis[0][0] = 1.;
    basis[0][1] = 0.;
    basis[0][2] = 0.;

    basis[1][0] = 2./3.;
    basis[1][1] = 1./3.;
    basis[1][2] = 0.5;

}