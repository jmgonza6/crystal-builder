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

    // apcell = 2;
    // ntype  = 1;
    // basis = new basis_t[apcell];
    // name = memory->new_1d<char>(4);
    // strcpy(name, "hcp");

    // a1.push_back(1.);
    // a1.push_back(0.);
    // a1.push_back(0.);

    // alpha = beta = 90.0;
    // gamma = 120.0;

    // a2 = MATHNS::rotate_vector(a1,"z",gamma);

    // a3.push_back(0.);
    // a3.push_back(0.);
    // a3.push_back(sqrt(8./3.));

    // basis[0].x = 1.;
    // basis[0].y = 0.;
    // basis[0].z = 0.;
    // basis[0].typeId = 1;

    // basis[1].x = 2./3.;
    // basis[1].y = 1./3.;
    // basis[1].z = 0.5;
    // basis[1].typeId = 1;

    // elemCount.push_back(2);

}