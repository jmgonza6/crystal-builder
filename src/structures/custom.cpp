#include "custom.h"
#include "../util/mathns.h"


Custom::Custom(double                              latPars[3],
               double                              latAngles[3],
               std::vector<std::vector<double> >   positions,
               std::string                         customName, 
               std::vector<std::string>            elemList,
               std::vector<int>                    typeCount) : LATTICE()
{
    char str[MAX_STRING_LENGTH];
    alat = latPars[0];
    blat = latPars[1];
    clat = latPars[2];

    alpha = latAngles[0];
    beta = latAngles[1];
    gamma = latAngles[2];

    
    ntype = elemList.size();


    apcell =0;
    for (int i=0;i<typeCount.size();i++) {
        apcell += typeCount[i];
        elemCount.push_back(typeCount[i]);

    }

    if (apcell==0) {printf ("ERRRRRRRRRRR: apcell == 0\n");exit(1);}

    
    name = memory->new_1d<char>(MAX_STRING_LENGTH);


    sprintf(str,"Custom, %s",customName.c_str());
    strcpy(name, str);

    // align a1 with the x axis
    a1.push_back(1.);
    a1.push_back(0.);
    a1.push_back(0.);

    // rotate a1 by γ to obtain a2
    a2 = MATHNS::rotate_vector( a1, "z", gamma );

    // rotate a1 by β to obtain a3
    a3 = MATHNS::rotate_vector( a1, "y", -beta );

    for (int s=0;s<elemList.size();s++) {
        species.push_back( elemList[s] );
    }


    basis = new basis_t[apcell];

    int count = 0;
    int n = 0;
    int id = 0;

    // loop over all types, assumed in stoichiometric order
    for (int t=0;t<ntype;t++){
        id = t;
        for (int i=0;i<typeCount[id];i++) {
            basis[n].x = positions[n][0];
            basis[n].y = positions[n][1];
            basis[n].z = positions[n][2];
            basis[n].typeId = id+1;
            n++;
        }
    }
    
}

Custom::~Custom()
{
    
}