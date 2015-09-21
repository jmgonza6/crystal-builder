#include "interface.h"


INTERFACE::INTERFACE(int nargs, char **argv, const char *p)
{
	lattice = NULL;
	fname = NULL;
	format = NULL;
	stylename = NULL;

	fname = new char[50];
	format = new char[50];

	write = new Write();
	errors = new Errors();
	gui = new Gui();

	banner();
}

INTERFACE::~INTERFACE()
{
	if (lattice) delete lattice;
	if (gui) delete gui;
	if (write) delete write;
	if (errors) delete errors;
}


void 
INTERFACE::banner()
{
	time_t now;
    time(&now);
    fprintf(SMESG, "___________________________________________________________\n");
	fprintf(SMESG, "%s Ver. %s\n",SOFT,VERS);
	fprintf(SMESG, "compiled on      %s\n",__DATE__);
	fprintf(SMESG, "Process started: !DATE %s", ctime(&now));
	fprintf(SMESG, "___________________________________________________________\n");
}


void 
INTERFACE::show_parameters()
{
    if (gui->pfile!=NULL) printf("parameters file = %s \n",gui->pfile);
    printf("bravais = %s \n",gui->bravais);
    printf("style = %s \n",gui->lstyle);
    printf("species = %s \n",gui->element);
	printf("alat = %f \n",gui->alat);

	printf("nx = %i \n",gui->nx);
	printf("ny = %i \n",gui->ny);
	printf("nz = %i \n",gui->nz);
	printf("format = %s \n",gui->file_format);	
	printf("out name = %s \n",gui->out_name);	
}


void 
INTERFACE::build()
{

	if (gui->custom) {
    	double latAngles[3], latPars[3];
    	latPars[0] = gui->alat;
    	latPars[1] = gui->blat;
    	latPars[2] = gui->clat;

    	latAngles[0] = gui->alpha;
    	latAngles[1] = gui->beta;
    	latAngles[2] = gui->gamma;

    	lattice = new Custom(latPars, latAngles, gui->customBasis, gui->customName, gui->elemList, gui->elemCount);

    } else if (strcmp(gui->bravais,"Cubic")==0) {
        lattice = new Cubic(gui->alat,gui->lstyle,gui->element);
    } else if (strcmp(gui->bravais,"Hexagonal")==0) {
        lattice = new Hexagonal(gui->alat, gui->element);
    } else if (strcmp(gui->bravais,"Tetragonal")==0) {
        lattice = new Tetragonal(gui->alat,gui->clat,gui->lstyle, gui->element);
    } else if (strcmp(gui->bravais,"Orthorhombic")==0) {
        lattice = new Orthorhombic(gui->alat,gui->blat, gui->clat, gui->lstyle, gui->element);
    } else if (strcmp(gui->bravais,"Monoclinic")==0) {
    	lattice = new Monoclinic(gui->alat,gui->blat, gui->clat, gui->beta, 4, 2, gui->lstyle, gui->element);
    } else if (strcmp(gui->bravais,"Library")==0) {
    	lattice = new StructureLib(gui->pd_struct,gui->lbasis,gui->alat,gui->cta,gui->elemList);
    } else {
    	errors->kill(FERR,"INTERFACE::build()",LERR,"Nothing bravais lattice selected, nothing to do, exiting");
    }


    // if (gui->bravais!=NULL) {
    // 	if (gui->custom==0 && strcmp(gui->bravais,"Library")!=0) {species.push_back(lattice->species[0]);}	
    // }


	lattice->nx = gui->nx;
	lattice->ny = gui->ny;
	lattice->nz = gui->nz;

	lattice->fractional = gui->fractional;
	lattice->build_crystal();


	if (DMSG) {
		printf("total atoms created %i\n\n",lattice->natom);
		for (int n=0;n<lattice->natom;n++) {
			printf ("C %f  %f  %f\n",lattice->coords[n][0],lattice->coords[n][1],lattice->coords[n][2]);
		}
	}

}

void 
INTERFACE::write_data()
{
	char comment[MAX_STRING_LENGTH];
	char mesg[MAX_STRING_LENGTH];
	char oformat[] = "cartesian";
	sprintf(comment,"[ (%dx%dx%d) %s crystal, a=%g, b=%g, c=%g ]", lattice->nx, lattice->ny, lattice->nz, lattice->name, lattice->alat, lattice->blat, lattice->clat);

	// reset fractional setting if writing to lammps
	// or dmol since they only read in cartesian
	if ((strcmp(gui->file_format,"lammps (.input)")==0 || strcmp(gui->file_format,"dmol (.car)")==0) && lattice->fractional==1) {

		sprintf(mesg,"Fractional coordinates were selected, but the [ %s ]\nformat does not allow this, using cartesian instead\n",gui->file_format);
		errors->warn(FERR,"INTERFACE::write_data()",LERR,mesg);

		lattice->fractional = 0;
	}

	lattice->convert_coordinates(lattice->fractional);

	if (strcmp(gui->file_format,"lammps (.input)")==0) {
		write->lammps(  gui->out_name, 
						comment, 
						lattice->natom, 
						lattice->ntype, 
						lattice->a1,
						lattice->a2,
						lattice->a3,
						lattice->type_list, 
						lattice->positions,
						lattice->species);
	} else if (strcmp(gui->file_format,"vasp (POSCAR)")==0) {
		char poscar[] = "POSCAR";
		write->poscar(  poscar,
						comment,
						lattice->type_count, 
						lattice->natom,  
						lattice->a1,
						lattice->a2,
						lattice->a3,
						lattice->positions,
						lattice->fractional,
						lattice->species);
	} else if (strcmp(gui->file_format,"dmol (.car)")==0) {
		std::vector<int> typeCount;

		
		write->dmol(gui->out_name, 
                    comment, 
                    lattice->natom, 
                    lattice->ntype, 
                    lattice->a1,
					lattice->a2,
					lattice->a3, 
                    lattice->type_list, 
                    lattice->positions,
                    lattice->species,
                    lattice->elemCount);  
    }
}


int 
INTERFACE::insane()
{
	#ifdef DEBUG
	printf ("ff %s\n",gui->file_format);
	printf ("on %s\n",gui->out_name);
	printf ("bv %s\n",gui->bravais);	
	printf ("e %s\n",gui->element);
	printf ("a %f\n",gui->alat);
	#endif

	// if we got the quit signal, no need to
	// check anything else
	if (gui->quit) return 1;

	// check for different parameters if the structure was custom
	if (gui->custom) {

		if (!gui->alat) {
			fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
			fprintf (EMESG,"  line %d -> lattice parameter 'a' not defined!\n",__LINE__);
			return 1;
		}

		if (!gui->blat) {
			fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
			fprintf (EMESG,"  line %d -> lattice parameter 'b' not defined!\n",__LINE__);
			return 1;
		}

		if (!gui->clat) {
			fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
			fprintf (EMESG,"  line %d -> lattice parameter 'c' not defined!\n",__LINE__);
			return 1;
		}

		if (!gui->alpha) {
			fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
			fprintf (EMESG,"  line %d -> lattice angle 'alpha' not defined!\n",__LINE__);
			return 1;
		}
	
		if (!gui->beta) {
			fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
			fprintf (EMESG,"  line %d -> lattice angle 'beta' not defined!\n",__LINE__);
			return 1;
		}
		if (!gui->gamma) {
			fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
			fprintf (EMESG,"  line %d -> lattice angle 'gamma' not defined!\n",__LINE__);
			return 1;
		}
		
		if (gui->elemList.empty()) {
			fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
			fprintf (EMESG,"  line %d -> no atomic species were defined!\n",__LINE__);
			return 1;
		}
		if (gui->customBasis.empty()) {
			fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
			fprintf (EMESG,"  line %d -> no basis atoms were defined!\n",__LINE__);
			return 1;
		}

		if (gui->out_name == NULL && strcmp(gui->file_format,"vasp (POSCAR)")!=0) {
			fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
			fprintf (EMESG,"  line %d -> no file name given for non VASP format!\n",__LINE__);
			return 1;
		}
		return 0;

	} else {
		// deal with the prebuilt structures first
		if (gui->graphene) {
			if (!gui->alat) {
	    		fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
				fprintf (EMESG,"  line %d -> lattice parameter 'a' was not defined!\n",__LINE__);
				return 1;
	    	}
	    	if (strcmp(gui->file_format,"vasp (POSCAR)")==0 
	    		|| strcmp(gui->file_format,"lammps (.input)")==0 
	    		|| strcmp(gui->file_format,"dmol (.car)")==0) { return 0; }
		}

		if (strcmp(gui->bravais,"Structures")==0 && strcmp(gui->file_format,"vasp (POSCAR)")==0) {return 0;}
		if (strcmp(gui->bravais,"Structures")==0 && strcmp(gui->file_format,"lammps (.input)")==0) {return 0;}
		if (strcmp(gui->bravais,"Structures")==0 && strcmp(gui->file_format,"dmol (.car)")==0) {return 0;}

		if (strcmp(gui->file_format,"vasp (POSCAR)")==0 && gui->alat !=0 && gui->element != NULL && gui->bravais != NULL) {return 0;}

	    // check that ALL necessary gui parameters are set
	    // if not, print which one was not set and return 1
	    if (gui->file_format==NULL) {
	    	fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
			fprintf (EMESG,"  line %d -> file format wasnt defined!\n",__LINE__);
			return 1;
	    }
	    // check for lattice specific values
	    if (!gui->bravais) {
	    	fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
			fprintf (EMESG,"  line %d -> bravais lattice was not defined!\n",__LINE__);
			return 1;
	    } else {
	    	if (strcmp(gui->bravais,"Cubic")==0 || strcmp(gui->bravais,"Hexagonal")==0) {
		    	if (!gui->alat) {
		    		fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
					fprintf (EMESG,"  line %d -> lattice parameter 'a' was not defined!\n",__LINE__);
					return 1;
		    	}
		    }

		    if (strcmp(gui->bravais,"Tetragonal")==0) {
		    	if (!gui->alat) {
		    		fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
					fprintf (EMESG,"  line %d -> lattice parameter 'a' was not defined!\n",__LINE__);
					return 1;
		    	}
		    	if (!gui->clat) {
		    		fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
					fprintf (EMESG,"  line %d -> lattice parameter 'c' was not defined!\n",__LINE__);
					return 1;
		    	}
		    }

		    if (strcmp(gui->bravais,"Orthorhombic")==0) {
		    	if (!gui->alat) {
		    		fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
					fprintf (EMESG,"  line %d -> lattice parameter 'a' was not defined!\n",__LINE__);
					return 1;
		    	}
		    	if (!gui->blat) {
		    		fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
					fprintf (EMESG,"  line %d -> lattice parameter 'b' was not defined!\n",__LINE__);
					return 1;
		    	}
		    	if (!gui->clat) {
		    		fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
					fprintf (EMESG,"  line %d -> lattice parameter 'c' was not defined!\n",__LINE__);
					return 1;
		    	}
		    }

		    if (strcmp(gui->bravais,"Monoclinic")==0) {
		    	if (!gui->alat) {
		    		fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
					fprintf (EMESG,"  line %d -> lattice parameter 'a' was not defined!\n",__LINE__);
					return 1;
		    	}
		    	if (!gui->blat) {
		    		fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
					fprintf (EMESG,"  line %d -> lattice parameter 'b' was not defined!\n",__LINE__);
					return 1;
		    	}
		    	if (!gui->clat) {
		    		fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
					fprintf (EMESG,"  line %d -> lattice parameter 'c' was not defined!\n",__LINE__);
					return 1;
		    	}
		    	if (!gui->beta) {
		    		fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
					fprintf (EMESG,"  line %d -> lattice angle 'beta' was not defined!\n",__LINE__);
					return 1;
		    	}
		    }

	    }
	    // if (!gui->element && gui->graphene == 0) {fprintf ("no elements defined! %s\n",gui->element);return 1;}
	    if (gui->out_name == NULL && strcmp(gui->file_format,"vasp (POSCAR)")!=0) {
	    	fprintf (EMESG,"INTERFACE::insane()::ERROR\n");
			fprintf (EMESG,"  line %d -> no file name given for non VASP format!\n",__LINE__);
			return 1;
	    }
	    return 0;
	}
}




// void INTERFACE::input(char *p)
// {
// 	std::ifstream parms(p);
// 	std::istringstream iss;
// 	std::string keyword, line;
// 	std::string lattice_class, istyle, ifmt, iname;
// 	char arg1[50], arg2[50], arg3[50], arg4[50];
// 	char arg5[50], arg6[50], arg7[50], arg8[50];

//     double lpar,cta;
//     int ex,ey,ez;

//     std::string modification, t1 = "empty", t2 = "empty", t3 = "empty", t4 = "empty";
//     double rx=-1, ry=-1, rz=-1;
//     double angles[3];
//     // phi      theta       psi
//     angles[0] = angles[1] = angles[2] = 0.;
//     bool rotate = false;
//     std::vector<std::string> species;
// 	int lbasis;

//     printf ("Reading from file ................: %s\n",p);

//     oformat = "cartesian";

//     while (std::getline(parms,line)) {
//         iss.clear();
//         iss.str(line);
//         iss >> keyword >> arg1 >> arg2 >> arg3 >> arg4 >> arg5 >> arg6 >> arg7 >> arg8;
//         if(keyword == "lattice") {
//             lattice_class = arg1;
//             istyle = arg2;
//             if (strncmp(arg3,"hex",3)==0 || strncmp(arg3,"hexagonal",9)==0) lbasis=1;
//             else if (strncmp(arg3,"ac",2)==0 || strncmp(arg3,"armchair",8)==0) lbasis = 3;
//             else if (strncmp(arg3,"zz",2)==0 || strncmp(arg3,"zigzag",6)==0) lbasis = 4;
//         	else lbasis = 3;
//         } else if (keyword == "lattice_modify") {
//             rotate = true;
//             modification = arg1;
//             if (modification == "rotate") {
// 	            if(atoi(arg2)) angles[0] = atof(arg2);
// 	            else angles[0] = 0.;
// 	            if(atoi(arg3)) angles[1] = atof(arg3);
// 	            else angles[1] = 0.;
// 	            if(atoi(arg4)) angles[2] = atof(arg4);
// 	            else angles[2] = 0.;
//         	}
//         } else if (keyword == "alat") {
//             lpar = atof(arg1);
//         } else if (keyword == "cta") {
//             cta = atof(arg1);
//         } else if (keyword == "output") {
//             iname = arg1;
//             ifmt = arg2;
//         } else if (keyword == "output_modify") {
//             oformat = arg1;
//         } else if (keyword == "dimension") {
//             ex = atoi(arg1);
//             ey = atoi(arg2);
//             ez = atoi(arg3);
//         } else if (keyword == "elements") {
//         	// need to pass this to each crystal constructor
//         	i_species = atoi(arg1);
//             t1 = arg2;
//             t2 = arg3;
//             t3 = arg4;
//             t4 = arg5;
//         }
//         line.clear();
//     }

//     if (t1 != "0") species.push_back(t1);
// 	if (t2 != "0") species.push_back(t2);
// 	if (t3 != "0") species.push_back(t3);
// 	if (t4 != "0") species.push_back(t4);

//     strcpy(format,ifmt.c_str());

//     fname = new char[iname.length()+1];
//     strcpy(fname,iname.c_str());

//     stylename = new char[istyle.length() + 1];
// 	strcpy(stylename, istyle.c_str());

//     #ifdef DEBUG
// 	    printf("lclass............................:  %s\n",lattice_class.c_str());
// 	    printf("lstyle............................:  %s\n",istyle.c_str());
// 	    printf("modification......................:  %s\n", modification.c_str());
// 	    if (rx!=-1)
// 	        printf("phi = %f     ",rx);
// 	        if (ry!=-1)
// 	            printf("theta = %f     ",ry );
// 	            if (rz!=-1)
// 	                printf("psi = %f\n",rz );
// 	    printf("\nformat............................:  %s\n",ifmt.c_str() );
// 	    printf("fname.............................:  %s\n", iname.c_str());
// 	    printf("alat..............................:  %f \n",lpar );
// 	    printf("dimension.........................:  %i %i %i \n",ex,ey,ez );
// 	    printf("cta...............................:  %f\n",cta);
//         printf("elements..........................:  ");
//         for(int ii=0; ii<i_species; ii++) printf("%s  ", species[ii].c_str());
// 	    printf("\n");
//     #endif

//     if (lattice_class == "bcc") {
//     	lclass = 1;
//     	lattice = new BCC(stylename, lpar);
//     	lattice->nx = ex; lattice->ny = ey; lattice->nz = ez;
//     	lattice->natom = lattice->nx*lattice->ny*lattice->nz*lattice->npcell;
//     	lattice->fformat = format;
//     	lattice->species = species;
//     	lattice->angles = angles;
//     } else if (lattice_class == "hcp") {
//     	lclass = 4;
//     	lattice = new Graphene(stylename, lpar, cta, lbasis);
//     	lattice->nx = ex; lattice->ny = ey; lattice->nz = ez;
//     	lattice->natom = lattice->nx*lattice->ny*lattice->nz*lattice->npcell;
//     	lattice->fformat = format;
//     	lattice->species = species;
//     	lattice->angles = angles;
//     } else if (lattice_class == "diamond") {
//     	lclass = 2;
//     	lattice = new DIAMOND(stylename, lpar);
//     	lattice->nx = ex; lattice->ny = ey; lattice->nz = ez;
//     	lattice->natom = lattice->nx*lattice->ny*lattice->nz*lattice->npcell;
//     	lattice->fformat = format;
//     	lattice->species = species;
//     	lattice->angles = angles;
//     } else {
//     	printf("ERROR: Unknown lattice class [ %s ] specified!\n",lattice_class.c_str());
//     	exit(1);
//     }

//     parms.close();
// }


