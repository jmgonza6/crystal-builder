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
    memory->new_1d(nt,ntype);

    apcell =0;
    for (int i=0;i<typeCount.size();i++) {
        apcell += typeCount[i];
        elemCount[i] = typeCount[i];

    }

    if (apcell==0) {printf ("ERRRRRRRRRRR: apcell == 0\n");exit(1);}

    

    memory->new_1d(name,MAX_STRING_LENGTH);


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


    
    memory->new_2d(basis,apcell,3);
    memory->new_1d(atom_type,apcell);

    int count = 0;
    int n = 0;

    // loop over all types, assumed in stoichiometric order
    for (int t=0;t<ntype;t++){
        int id = t;
        for (int i=0;i<typeCount[t];i++) {
            atom_type[n] = id +1;
            basis[n][0] = positions[n][0];
            basis[n][1] = positions[n][1];
            basis[n][2] = positions[n][2];
            n++;
        }
    }
    
}

Custom::~Custom()
{
    
}