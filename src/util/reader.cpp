#include "reader.h"

Reader::Reader()
{
    parser = new Parser();

    out_name = NULL;
    file_format = NULL;
    bravais = NULL;

    alat=blat=clat=1;
    alpha=beta=gamma=90;

    nx=ny=nz=1;
    fractional=0;
    graphene=0;

    custom=0;
    view=0;

}

Reader::~Reader()
{
    delete parser;
}



void 
Reader::file(char *pf)
{
	std::ifstream pfile(pf);
    if (!pfile.is_open()) {printf ("Could not open the parameters file [ %s ]\n",pf);}

    double x,y,z;
    int bflag=0;


	std::istringstream iss;
	std::string keyword, line;
	std::string arg1, arg2, arg3;

    std::string blattice;
    std::string ffmt, fname;

    std::vector<std::string> keys, vals;
    std::vector<std::string> g_elemList;
    std::vector<std::string> g_elemCount;
    std::vector<std::string> basisList;


	if (DMSG)
    	fprintf (FDEBUG,"Reading from file ................: %s\n",pf);


    // search first for the required parameters
    while (std::getline(pfile,line)) {
        iss.clear();
        iss.str(line);
        iss >> keyword >> arg1 >> arg2;;

        if (keyword == "bravais") {
            bravais = parser->str2char(arg1);
        	blattice = arg1;
        } else if (keyword == "save") {
        	ffmt = arg1;
            file_format = parser->str2char(arg1);
        	if (ffmt == "vasp") {
        		fname = "POSCAR";
                out_name = parser->str2char("POSCAR");
        	} else {
                out_name = parser->str2char(arg2);
        		fname = arg2;
        	}
        } else if (keyword == "bravais_style") {
            lstyle = parser->str2char(arg1);
        }           
    }



    pfile.clear();                 // clear fail and eof bits
	pfile.seekg(0, std::ios::beg); // back to the start!

	// search for general modifications
	while (std::getline(pfile,line)) {
        iss.clear();
        iss.str(line);
        iss >> keyword >> arg1 >> arg2;

        if (keyword == "stoich") {
      		parser->get_substrings(arg1, ',', ':', g_elemList, g_elemCount);
            elemList = g_elemList;
            for (int i=0;i<g_elemCount.size();i++) {
                elemCount.push_back(parser->str2num<int>(g_elemCount[i]));
            }
			keyword = "";
        } else if (keyword == "lat_parameters") {
        	parser->get_substrings(arg1, ',', '=', keys, vals);
        	for(int i=0; i<keys.size();i++) {
        		if (keys[i]=="a") alat = parser->str2num<double>(vals[i]);
        		else if (keys[i]=="b") blat = parser->str2num<double>(vals[i]);
        		else if (keys[i]=="c") clat = parser->str2num<double>(vals[i]);
        	}
        	keyword = "";
        } else if (keyword == "lat_angles") {
        	parser->get_substrings(arg1, ',', '=', keys, vals);
        	for(int i=0; i<keys.size();i++) {
        		if (keys[i]=="alpha") alpha = parser->str2num<double>(vals[i]);
        		else if (keys[i]=="beta") beta = parser->str2num<double>(vals[i]);
        		else if (keys[i]=="gamma") gamma = parser->str2num<double>(vals[i]);
        	}
        	keyword = "";
        } else if (keyword == "super_cell") {
        	parser->get_substrings(arg1, ',', '=', keys, vals);
        	for(int i=0; i<keys.size();i++) {
        		if (keys[i]=="nx") nx = parser->str2num<int>(vals[i]);
        		else if (keys[i]=="ny") ny = parser->str2num<int>(vals[i]);
        		else if (keys[i]=="nz") nz = parser->str2num<int>(vals[i]);
        	}
        	keyword = "";
        } else if (keyword == "data_modify") {
        	if (arg1 == "fractional") {
        		fractional = 1;
        	}
        	keyword = "";
        } else if (keyword == "run") {
        	if (arg1 == "build") {
        		int build = 1;
        	} else if (arg1 == "render") {
        		view = 1;
        	} else {
        		printf ("ERROR: unknown run style [ %s ]\n",arg1.c_str());
        	}
        	keyword = "";
        } else if (keyword == "bravais_style") {
            lstyle = parser->str2char(arg1);
        }
    }

	pfile.clear();                 // clear fail and eof bits
	pfile.seekg(0, std::ios::beg); // back to the start!

   // search for library relevant parameters aside from the general modifiers
	if (blattice == "library") {
		while (std::getline(pfile,line)) {
	        iss.clear();
	        iss.str(line);
	        iss >> keyword >> arg1;

	        if (keyword == "use_prebuilt") {
	        	if (!parser->str2num<int>(arg1)) {
                    if (arg1 == "graphene") {
                        pd_struct = parser->str2char("graphene");
                        graphene=1;
                    } else if (arg1 == "hbn") {
                        pd_struct = parser->str2char("h-BN");
                    } else if (arg1 == "mdc-2h") {
                        pd_struct = parser->str2char("MDC MX2-2H");
                        if (elemList.empty()) {
                            elemList.push_back("Sn");
                            elemList.push_back("S");
                            elemCount.push_back(1);
                            elemCount.push_back(2);
                        } 
                        if (alat==1) alat = 3.684;

                        if (clat==1) clat = 5.89;
                    } else if (arg1 == "mdc-1t") {
                        pd_struct = parser->str2char("MDC MX2-1T");
                        if (elemList.empty()) {
                            elemList.push_back("Sn");
                            elemList.push_back("S");
                            elemCount.push_back(1);
                            elemCount.push_back(2);
                        } 
                        if (alat==1) alat = 3.684;

                        if (clat==1) clat = 5.89;
                    } else if (arg1 == "tmdc-2h") {
                        pd_struct = parser->str2char("TMDC MX2-2H");
                        if (elemList.empty()) {
                            elemList.push_back("Mo");
                            elemList.push_back("S");
                            elemCount.push_back(1);
                            elemCount.push_back(2);
                        } 
                        if (alat==1) alat = 3.19;

                        if (clat==1) clat = 12.3;
                    } else if (arg1 == "tmdc-1t") {
                        pd_struct = parser->str2char("TMDC MX2-1T");
                        if (elemList.empty()) {
                            elemList.push_back("Mo");
                            elemList.push_back("S");
                            elemCount.push_back(1);
                            elemCount.push_back(2);
                        } 
                        if (alat==1) alat = 3.19;

                        if (clat==1) clat = 12.3;
                    } else if (arg1 == "petn") {
                        pd_struct = parser->str2char("PETN-I");
                        elemList.clear();
                        elemCount.clear();
                        elemList.push_back("C");
                        elemList.push_back("H");
                        elemList.push_back("N");
                        elemList.push_back("O");
                        elemCount.push_back(10);
                        elemCount.push_back(16);
                        elemCount.push_back(8);
                        elemCount.push_back(24);
                    } else if (arg1 == "tatb") {
                        pd_struct = parser->str2char("TATB");
                        elemList.clear();
                        elemCount.clear();
                        elemList.push_back("C");
                        elemList.push_back("H");
                        elemList.push_back("N");
                        elemList.push_back("O");
                        elemCount.push_back(12);
                        elemCount.push_back(12);
                        elemCount.push_back(12);
                        elemCount.push_back(12);
                    } else if (arg1 == "hmx") pd_struct = parser->str2char("B-HMX");
                    
	        	} else {
                    printf ("prebuilt index = %i\n",parser->str2num<int>(arg1));
                    int index = parser->str2num<int>(arg1);
                    if (index == 1) {
                        pd_struct = parser->str2char("graphene");
                        graphene=1;
                    } else if (index == 2) {
                        pd_struct = parser->str2char("h-BN");
                    } else if (index == 3) {
                        pd_struct = parser->str2char("MDC MX2-2H");
                        if (elemList.empty()) {
                            elemList.push_back("Sn");
                            elemList.push_back("S");
                            elemCount.push_back(1);
                            elemCount.push_back(2);
                        } 
                        if (alat==1) alat = 3.684;

                        if (clat==1) clat = 5.89;
                    } else if (index == 4) {
                        pd_struct = parser->str2char("MDC MX2-1T");
                        if (elemList.empty()) {
                            elemList.push_back("Sn");
                            elemList.push_back("S");
                            elemCount.push_back(1);
                            elemCount.push_back(2);
                        } 
                        if (alat==1) alat = 3.684;

                        if (clat==1) clat = 5.89;
                    } else if (index == 5) {
                        pd_struct = parser->str2char("TMDC MX2-2H");
                        if (elemList.empty()) {
                            elemList.push_back("Mo");
                            elemList.push_back("S");
                            elemCount.push_back(1);
                            elemCount.push_back(2);
                        } 
                        if (alat==1) alat = 3.19;

                        if (clat==1) clat = 12.3;
                    } else if (index == 6) {
                        pd_struct = parser->str2char("TMDC MX2-1T");
                        if (elemList.empty()) {
                            elemList.push_back("Mo");
                            elemList.push_back("S");
                            elemCount.push_back(1);
                            elemCount.push_back(2);
                        } 
                        if (alat==1) alat = 3.19;

                        if (clat==1) clat = 12.3;
                    } else if (index == 7) {
                        pd_struct = parser->str2char("PETN-I");
                        elemList.clear();
                        elemCount.clear();
                        elemList.push_back("C");
                        elemList.push_back("H");
                        elemList.push_back("N");
                        elemList.push_back("O");
                        elemCount.push_back(10);
                        elemCount.push_back(16);
                        elemCount.push_back(8);
                        elemCount.push_back(24);
                    } else if (index == 8) {
                        pd_struct = parser->str2char("TATB");
                        elemList.clear();
                        elemCount.clear();
                        elemList.push_back("C");
                        elemList.push_back("H");
                        elemList.push_back("N");
                        elemList.push_back("O");
                        elemCount.push_back(12);
                        elemCount.push_back(12);
                        elemCount.push_back(12);
                        elemCount.push_back(12);
                    } else if (index == 9) {
                        pd_struct = parser->str2char("B-HMX");
                    }
                }
	        		
	        } else if (keyword == "graphene_basis") {
                lbasis = parser->str2num<int>(arg1);
            }
	        keyword = "";
	    }
	} else if (blattice == "cubic") {
		blat = clat = alat;
		alpha = beta = gamma = 90;
	} else if (blattice == "tetragonal") {
		blat = alat;
		alpha = beta = gamma = 90;
	} else if (blattice == "ortho") {
		alpha = beta = gamma = 90;
	} 


    // if a custom crystal was requested, then search for the basis atoms
    if (blattice == "custom") {
        custom = 1;
        pfile.clear();                 // clear fail and eof bits
        pfile.seekg(0, std::ios::beg); // back to the start!

        std::vector<double> row(3);

        bflag = 0;
        while (std::getline(pfile,line)) {
            iss.clear();
            iss.str(line);
            iss >> arg1 >> arg2 >> arg3;
            if (arg1 == "begin_basis") {
                arg1 = "";
                while (std::getline(pfile,line)) {
                    iss.clear();
                    iss.str(line);
                    iss >> arg1 >> arg2 >> arg3;
                    // stop reading if we have all the atoms
                    if (arg1 == "end_basis") {bflag=1;break;}

                    row[0] = parser->str2num<double>(arg1);
                    row[1] = parser->str2num<double>(arg2);
                    row[2] = parser->str2num<double>(arg3);
                    
                    arg1 = "";
                    customBasis.push_back(row);
                }
                if (bflag) break;
            }
        }

        // create a name for the structure based on its stoichiometry
        std::string unit;
        for (int i=0;i<g_elemList.size();i++) {
            // if there is only 1 of a species, print only the element symbol
            if (parser->str2num<int>(g_elemCount[i])==1) unit = g_elemList[i];
            else unit = g_elemList[i] + g_elemCount[i];
            customName += unit;
        }
    }

    if (DMSG) {
        printf ("blattice = %s\n",bravais);
        printf ("ifmt = %s\n",file_format);
        printf ("fname = %s\n",out_name);
        printf ("latticeStyle = %s\n",lstyle);
        for (int i=0;i<elemList.size();i++)
            printf ("stoich = %s %i\n",elemList[i].c_str(),elemCount[i]);


        printf ("nx    = %i\n",nx);
        printf ("ny    = %i\n",ny);
        printf ("nz    = %i\n",nz);
        printf ("alat  = %f\n",alat);
        printf ("blat  = %f\n",blat);
        printf ("clat  = %f\n",clat);
        printf ("alpha = %f\n",alpha);
        printf ("beta  = %f\n",beta);
        printf ("gamma = %f\n",gamma);

    }
}

int 
Reader::insane()
{

    // check for different parameters if the structure was custom
    if (custom) {

        if (!alat) {
            fprintf (EMESG,"Reader::insane()::ERROR\n");
            fprintf (EMESG,"  line %d -> lattice parameter 'a' not defined!\n",__LINE__);
            return 1;
        }

        if (!blat) {
            fprintf (EMESG,"Reader::insane()::ERROR\n");
            fprintf (EMESG,"  line %d -> lattice parameter 'b' not defined!\n",__LINE__);
            return 1;
        }

        if (!clat) {
            fprintf (EMESG,"Reader::insane()::ERROR\n");
            fprintf (EMESG,"  line %d -> lattice parameter 'c' not defined!\n",__LINE__);
            return 1;
        }

        if (!alpha) {
            fprintf (EMESG,"Reader::insane()::ERROR\n");
            fprintf (EMESG,"  line %d -> lattice angle 'alpha' not defined!\n",__LINE__);
            return 1;
        }
    
        if (!beta) {
            fprintf (EMESG,"Reader::insane()::ERROR\n");
            fprintf (EMESG,"  line %d -> lattice angle 'beta' not defined!\n",__LINE__);
            return 1;
        }
        if (!gamma) {
            fprintf (EMESG,"Reader::insane()::ERROR\n");
            fprintf (EMESG,"  line %d -> lattice angle 'gamma' not defined!\n",__LINE__);
            return 1;
        }
        
        if (elemList.empty()) {
            fprintf (EMESG,"Reader::insane()::ERROR\n");
            fprintf (EMESG,"  line %d -> no atomic species were defined!\n",__LINE__);
            return 1;
        }
        if (customBasis.empty()) {
            fprintf (EMESG,"Reader::insane()::ERROR\n");
            fprintf (EMESG,"  line %d -> no basis atoms were defined!\n",__LINE__);
            return 1;
        }

        if (out_name == NULL && strcmp(file_format,"vasp (POSCAR)")!=0) {
            fprintf (EMESG,"Reader::insane()::ERROR\n");
            fprintf (EMESG,"  line %d -> no file name given for non VASP format!\n",__LINE__);
            return 1;
        }
        return 0;

    } else {
        // deal with the prebuilt structures first
        if (graphene) {
            if (!alat) {
                fprintf (EMESG,"Reader::insane()::ERROR\n");
                fprintf (EMESG,"  line %d -> lattice parameter 'a' was not defined!\n",__LINE__);
                return 1;
            }
            if (strcmp(file_format,"vasp")==0 
                || strcmp(file_format,"lammps")==0 
                || strcmp(file_format,"dmol")==0) { return 0; }
        }

        if (strcmp(bravais,"library")==0 && strcmp(file_format,"vasp")==0) {return 0;}
        if (strcmp(bravais,"library")==0 && strcmp(file_format,"lammps")==0) {return 0;}
        if (strcmp(bravais,"library")==0 && strcmp(file_format,"dmol ")==0) {return 0;}

        if (strcmp(file_format,"vasp")==0 && alat !=0 && !elemList.empty() && bravais != NULL) {return 0;}

        // check that ALL necessary gui parameters are set
        // if not, print which one was not set and return 1
        if (file_format==NULL) {
            fprintf (EMESG,"Reader::insane()::ERROR\n");
            fprintf (EMESG,"  line %d -> file format wasnt defined!\n",__LINE__);
            return 1;
        }
        // check for lattice specific values
        if (!bravais) {
            fprintf (EMESG,"Reader::insane()::ERROR\n");
            fprintf (EMESG,"  line %d -> bravais lattice was not defined!\n",__LINE__);
            return 1;
        } else {
            if (strcmp(bravais,"cubic")==0 || strcmp(bravais,"hexagonal")==0) {
                if (!alat) {
                    fprintf (EMESG,"Reader::insane()::ERROR\n");
                    fprintf (EMESG,"  line %d -> lattice parameter 'a' was not defined!\n",__LINE__);
                    return 1;
                }
            }

            if (strcmp(bravais,"tetragonal")==0) {
                if (!alat) {
                    fprintf (EMESG,"Reader::insane()::ERROR\n");
                    fprintf (EMESG,"  line %d -> lattice parameter 'a' was not defined!\n",__LINE__);
                    return 1;
                }
                if (!clat) {
                    fprintf (EMESG,"Reader::insane()::ERROR\n");
                    fprintf (EMESG,"  line %d -> lattice parameter 'c' was not defined!\n",__LINE__);
                    return 1;
                }
            }

            if (strcmp(bravais,"ortho")==0) {
                if (!alat) {
                    fprintf (EMESG,"Reader::insane()::ERROR\n");
                    fprintf (EMESG,"  line %d -> lattice parameter 'a' was not defined!\n",__LINE__);
                    return 1;
                }
                if (!blat) {
                    fprintf (EMESG,"Reader::insane()::ERROR\n");
                    fprintf (EMESG,"  line %d -> lattice parameter 'b' was not defined!\n",__LINE__);
                    return 1;
                }
                if (!clat) {
                    fprintf (EMESG,"Reader::insane()::ERROR\n");
                    fprintf (EMESG,"  line %d -> lattice parameter 'c' was not defined!\n",__LINE__);
                    return 1;
                }
            }
        }
        if (out_name == NULL && strcmp(file_format,"vasp")!=0) {
            fprintf (EMESG,"Reader::insane()::ERROR\n");
            fprintf (EMESG,"  line %d -> no file name given for non VASP format!\n",__LINE__);
            return 1;
        }
        return 0;
    }
}
