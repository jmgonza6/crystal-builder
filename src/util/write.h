/** 
  * \class Write write.h "write.h"
  *
  * \b Purpose:
  *  =============
  \verbatim 
  /********************************************************************************\
  /  Class for writing atomic crystal structure data in various DFT and MD formats \
  /  Currently, this class supports the following file formats:                    \
  /          MSI/DMol .car format,                                                 \
  /          LAMMPS input format,                                                  \
  /          VASP 5.x.x POSCAR format, Direct or Cartesian, no Selective Dynamics  \
  /                                                                                \
  /********************************************************************************\
  \endverbatim
  *
  * \author Joseph M. Gonzalez
  *
  * \version 0.1
  *
  * \date Sep 13, 2015 19:16:20
  *
  * \b Contact \n    jmgonza6@mail.usf.edu
  *
  */


#ifndef Write_H
#define Write_H

#include "common.h"

class Write {

	private:
		Memory *memory;

		std::string 
		mass2elem(double mass);
		
		double 
		elem2mass(std::string element);


	
	public:
		Write();
		~Write();
  
  		/** \brief Write the crystal structure defined by the user in the DMol `.car` format
		  * \param[in] outname - name of the regular file for writing
		  * \param[in] comment - a comment line describing the crystal
		  * \param[in] natoms - total number of atoms created
		  * \param[in] ntype - total number of unique elements
		  * \param[in] a1 - major lattice vector along x
		  * \param[in] a2 - lattice vector in xy plane
		  * \param[in] a3 - lattice vector in xyz plane
		  * \param[in] atomIds - list of all atomic types, corresponds to particle ID
		  * \param[in] coords - 2D array containing the atomic cooridinates in cartesian or fractional units
		  * \param[in] species - list of all unique element strings
		  */
		void 
		dmol(char                      outname[], 
             char                      comment[], 
             int                       natoms, 
             int                       ntype, 
             std::vector<double>       a1, 
             std::vector<double>       a2, 
             std::vector<double>       a3, 
             int                       *atom_ids, 
             double                    **coords, 
             std::vector<std::string>  speciesList,
             std::vector<int>          speciesCount);

		/** \brief Write the crystal structure defined by the user in the LAMMPS `.input` format
		  * \param[in] outname - name of the regular file for writing
		  * \param[in] comment - a comment line describing the crystal
		  * \param[in] natoms - total number of atoms created
		  * \param[in] ntype - total number of unique elements
		  * \param[in] a1 - major lattice vector along x
		  * \param[in] a2 - lattice vector in xy plane
		  * \param[in] a3 - lattice vector in xyz plane
		  * \param[in] atomIds - list of all atomic types, corresponds to particle ID
		  * \param[in] coords - 2D array containing the atomic cooridinates in cartesian or fractional units
		  * \param[in] species - list of all unique element strings
		  */
		void 
		lammps( char                       outname[], 
		        char                       comment[], 
		        int                        natoms, 
		        int                        ntype, 
		        std::vector<double>        a1, 
				std::vector<double>        a2, 
				std::vector<double>        a3,  
		        int                        *atomIds, 
		        double                     **coords, 
		        std::vector<std::string>   species);

		/** \brief Write the crystal structure defined by the user in the VASP `POSCAR` format
		  * \param[in] outname - name of the regular file for writing
		  * \param[in] comment - a comment line describing the crystal
		  * \param[in] typeCount - pointer to array which keeps track of total number of each unique type
		  * \param[in] natoms - total number of atoms created
		  * \param[in] a1 - major lattice vector along x
		  * \param[in] a2 - lattice vector in xy plane
		  * \param[in] a3 - lattice vector in xyz plane
		  * \param[in] coords - 2D array containing the atomic cooridinates in cartesian or fractional units
		  * \param[in] fractional - `coords` is in fractional units
		  * \arg `1` = fractional `0` = cartesian
		  * \param[in] species - list of all unique element strings
		  */
		void 
		poscar(	char                       outname[],
			    char                       comment[], 
				int                        *typeCount, 
				int                        natoms, 
				std::vector<double>        a1, 
				std::vector<double>        a2, 
				std::vector<double>        a3, 
				double                     **coords, 
				int                        fractional, 
				std::vector<std::string>   species);
};
#endif