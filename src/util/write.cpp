#include "write.h"
#include "mathns.h"

PeriodicTable ptable = 
    /* Elements list */ //std::string
    {"EM",
     "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", 
     "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", 
     "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn",
     "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", 
     "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn",
     "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd",
     "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb",
     "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg",
     "Ti", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th",
     "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm",
     "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt",
    /* Atomic masses */                 
    {0.,
     1.0079, 4.0026, 6.941, 9.0122, 10.811, 12.011, 14.007, 15.999, 18.998, 20.180, 
     22.990, 24.305, 26.982, 28.086, 30.974, 32.065, 35.453, 39.948, 39.098, 40.078, 
     44.956, 47.867, 50.942, 51.996, 54.938, 55.845, 58.933, 58.693, 63.546, 65.390,
     69.723, 72.610, 74.992, 78.960, 79.904, 83.800, 85.468, 87.620, 88.906, 91.224,
     92.906, 95.940, 98.000, 101.070, 102.910, 106.420, 107.87, 112.41, 114.82, 118.71,
     121.76, 127.60, 126.90, 131.29, 132.91, 137.33, 138.91, 140.12, 140.91, 144.24,
     145.00, 150.36, 151.96, 157.25, 158.93, 162.50, 164.93, 167.26, 168.93, 173.04,
     174.97, 178.49, 180.95, 183.84, 186.21, 190.23, 192.22, 195.08, 196.97, 200.59,
     204.38, 207.20, 208.98, 209.00, 210.00, 222.00, 223.00, 226.00, 227.00, 232.04,
     231.04, 238.03, 237.00, 244.00, 243.00, 247.00, 247.00, 251.00, 252.00, 257.00,
     258.00, 259.00, 262.00, 261.00, 262.00, 266.00, 264.00, 269.00, 268.00
    }
    };


/* ----------------------------------------------------------------------
   constructor does nothing
------------------------------------------------------------------------- */
Write::Write()
{
    memory = new Memory();
  
}

/* ----------------------------------------------------------------------
   deconstructor destroy any allocated memory
------------------------------------------------------------------------- */
Write::~Write()
{
    delete memory;
}

std::string 
Write::mass2elem(double mass)
{
    double epsilon = 0.05, upper, lower;
    for(int ii=1;ii<110;++ii) {
        upper = ptable.amu[ii] + epsilon;
        lower = ptable.amu[ii] - epsilon;
        if( (lower < mass) && (mass < upper) ) return ptable.elements[ii];
    }
}

double 
Write::elem2mass(std::string element)
{
    int ii;
    for(ii=1;ii<110;++ii) 
        if (element == ptable.elements[ii]) return ptable.amu[ii];
}


void 
Write::lammps(   char                      outname[], 
                 char                      comment[], 
                 int                       natom, 
                 std::vector<double>       a1, 
                 std::vector<double>       a2, 
                 std::vector<double>       a3,  
                 atom_t*                   atom, 
                 std::vector<std::string>  speciesList)
{
    if (DMSG) fprintf (FDEBUG, "Writing inside lammps\n");

    int nt = speciesList.size();
    
    FILE *fout = outname ? fopen(outname, "w" ) : NULL;
    double zero = 0.;
    double mass=0;
    fprintf(fout,"%s\n\n",comment );
    fprintf(fout, "%d  atoms\n\n", natom);
    fprintf(fout, "%d  atom types\n\n", nt);
    fprintf(fout, "%18.12f %18.12f  xlo xhi\n", zero, a1[0]);
    fprintf(fout, "%18.12f %18.12f  ylo yhi\n", zero, a2[1]);
    fprintf(fout, "%18.12f %18.12f  zlo zhi\n", zero, a3[2]);
    if ( a1[0]*a1[0] + a3[0]*a3[0] + a3[1]*a3[1] > 0 )
      fprintf(fout, "%18.12f %18.12f %18.12f xy xz yz\n", a2[0], a3[0], a3[1]);

    double alpha, beta, gamma;
    double a,b,c;

    a = MATHNS::vector_mag(a1);
    b = MATHNS::vector_mag(a2);
    c = MATHNS::vector_mag(a3);

    alpha = MATHNS::angle(a2,a3);
    beta = MATHNS::angle(a1,a3);
    gamma = MATHNS::angle(a1,a2);

    if (DMSG) fprintf (FDEBUG, "alpha %f | beta %f | gamma %f\n",alpha*R2D,beta*R2D,gamma*R2D);
    

    if (DMSG) fprintf (FDEBUG, "Writing header\n");
    

    // Add method to print masses based on user defined types
    fprintf(fout,"\nMasses\n\n");

    for(int m=0; m<nt;m++) {
        mass = elem2mass(speciesList[m]);
        fprintf(fout,"%i  %f\n",m+1, mass);
    }
    // fprintf(fout,"\n");
    fprintf(fout, "\nAtoms\n\n");
    for (int i=0; i<natom; i++) fprintf(fout,"%5d %3d %18.12f %18.12f %18.12f\n", i+1, atom[i].typeId, atom[i].x, atom[i].y, atom[i].z);
    if (fout) fclose(fout);
}


void 
Write::poscar(  char                     comment[],   
                int                      natoms, 
                std::vector<double>      a1, 
                std::vector<double>      a2, 
                std::vector<double>      a3, 
                atom_t                   *atom, 
                int                      fractional, 
                std::vector<std::string> speciesList,
                std::vector<int>         speciesCount)
{
    char com_line[MAX_STRING_LENGTH];

    std::string tmp,st_str;

    for (int s=0;s<speciesList.size();s++) {
        st_str = st_str + " " + speciesList[s];
    }
    sprintf(com_line,"%s %s",st_str.c_str(), comment);

    if (DMSG) fprintf (FDEBUG, "WRITE::DEBUG:  >>  Writing inside poscar\n");
    
    FILE *fout = fopen("POSCAR","w");
    double lscale = 1.;
    
    double alpha, beta, gamma;
    double a,b,c;

    a = MATHNS::vector_mag(a1);
    b = MATHNS::vector_mag(a2);
    c = MATHNS::vector_mag(a3);

    alpha = MATHNS::angle(a2,a3);
    beta = MATHNS::angle(a1,a3);
    gamma = MATHNS::angle(a1,a2);


    if (DMSG) fprintf (FDEBUG, "alpha %f | beta %f | gamma %f\n",alpha*R2D,beta*R2D,gamma*R2D);
    

    if (DMSG) fprintf (FDEBUG, "Writing header\n");
    
    // print the new POSCAR
    fprintf(fout,"%s\n", com_line);
    fprintf(fout,"%18.14f\n", lscale);
    fprintf(fout,"  %18.12f  %18.12f  %18.12f\n", a1[0], a1[1], a1[2]);
    fprintf(fout,"  %18.12f  %18.12f  %18.12f\n", a2[0], a2[1], a2[2]);
    fprintf(fout,"  %18.12f  %18.12f  %18.12f\n", a3[0], a3[1], a3[2]);

    for(int n=0;n<speciesList.size();n++) {
        fprintf(fout,"%2i ",speciesCount[n]);
    }
    fprintf(fout,"\n");

    if (DMSG) fprintf (FDEBUG, "done\n");
    
    
    if (fractional) fprintf(fout,"Direct\n");
    else fprintf(fout,"Cartesian\n");
    
    for (int n=0;n<natoms;n++) {
        fprintf(fout,"%18.12f   %18.12f   %18.12f\n",atom[n].x,atom[n].y,atom[n].z);
    }
    if(fout) fclose(fout);
}


void 
Write::dmol(char                      outname[], 
            int                       natoms, 
            std::vector<double>       a1, 
            std::vector<double>       a2, 
            std::vector<double>       a3, 
            atom_t*                   atom, 
            std::vector<std::string>  speciesList,
            std::vector<int>          speciesCount)
{
    if (DMSG) fprintf (FDEBUG, "Writing inside dmol\n");
    

    char numstr[21];
    std::string elemId,p1, p2;
    double Q = 0.;
    p1 = "XXXX";
    p2 = "xx";
    int typ = 1;

    time_t now;
    time(&now);

    /* Print the car header */
    FILE *outF = fopen(outname,"w");
    fprintf(outF,"!BIOSYM archive 3\n");
    fprintf(outF,"PBC=ON\n");
    fprintf(outF,"CAR File prepared by %s v %s\n",SOFT,VERS);
    fprintf(outF,"!DATE %s", ctime(&now));

    double alpha, beta, gamma;
    double a,b,c;

    a = MATHNS::vector_mag(a1);
    b = MATHNS::vector_mag(a2);
    c = MATHNS::vector_mag(a3);

    alpha = MATHNS::angle(a2,a3);
    beta = MATHNS::angle(a1,a3);
    gamma = MATHNS::angle(a1,a2);

    if (DMSG) fprintf (FDEBUG, "alpha %f | beta %f | gamma %f\n",alpha*R2D,beta*R2D,gamma*R2D);
    

    fprintf(outF,"PBC  %8.4f  %8.4f  %8.4f  %8.4f  %8.4f %8.4f  (P1)\n",\
     a,b,c,R2D*alpha,R2D*beta,R2D*gamma);

    int n = 0;
    int id = 0;
    // loop over all types, assumed in stoichiometric order
    for (int t=0;t<speciesList.size();t++){
        id = t;
        for (int i=0;i<speciesCount[id];i++) {
            sprintf(numstr, "%d", n+1);
            elemId = speciesList[id] + numstr;
            fprintf(outF, "%-7s %12.8lf   %12.8lf   %12.8lf %4s %-6i %-7s %s %7.3lf\n",\
                elemId.c_str(),atom[n].x,atom[n].y,atom[n].z,p1.c_str(),typ,p2.c_str(),speciesList[id].c_str(),Q);
            n++;
        }
    }

}

