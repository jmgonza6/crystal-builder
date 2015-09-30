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
		  * \param[in] natoms - total number of atoms created
		  * \param[in] a1 - major lattice vector along x
		  * \param[in] a2 - lattice vector in xy plane
		  * \param[in] a3 - lattice vector in xyz plane
		  * \param[in] atoms - Structure containing the atomic positions, and atomic type numbers
		  * \param[in] speciesList - list of all unique elements
		  * \param[in] speciesCount - 1D vector containing the count of each type in `speciesList`
		  */
		void 
		dmol(char                      outname[], 
             int                       natoms, 
             std::vector<double>       a1, 
             std::vector<double>       a2, 
             std::vector<double>       a3, 
             atom_t*                   atoms, 
             std::vector<std::string>  speciesList,
             std::vector<int>          speciesCount);

		/** \brief Write the crystal structure defined by the user in the LAMMPS `.input` format
		  * \param[in] outname - name of the regular file for writing
		  * \param[in] comment - a comment line describing the crystal
		  * \param[in] natoms - total number of atoms created
		  * \param[in] a1 - major lattice vector along x
		  * \param[in] a2 - lattice vector in xy plane
		  * \param[in] a3 - lattice vector in xyz plane
		  * \param[in] atoms - Structure containing the atomic positions, and atomic type numbers
		  * \param[in] speciesList - list of all unique element strings
		  */
		void 
		lammps( char                       outname[], 
		        char                       comment[], 
		        int                        natoms, 
		        std::vector<double>        a1, 
				std::vector<double>        a2, 
				std::vector<double>        a3,  
		        atom_t*                    atoms, 
		        std::vector<std::string>   speciesList);

		/** \brief Write the crystal structure defined by the user in the VASP `POSCAR` format
		  * \param[in] comment - a comment line describing the crystal
		  * \param[in] natoms - total number of atoms created
		  * \param[in] a1 - major lattice vector along x
		  * \param[in] a2 - lattice vector in xy plane
		  * \param[in] a3 - lattice vector in xyz plane
		  * \param[in] atoms - Structure containing the atomic positions, and atomic type numbers
		  * \param[in] fractional - `coords` is in fractional units
		  * \arg `1` = fractional `0` = cartesian
		  * \param[in] speciesList - list of all unique element strings
		  * \param[in] speciesCount - 1D vector containing the count of each type in `speciesList`
		  */
		void 
		poscar(	char                       comment[],  
				int                        natoms, 
				std::vector<double>        a1, 
				std::vector<double>        a2, 
				std::vector<double>        a3, 
				atom_t*                    atoms, 
				int                        fractional, 
				std::vector<std::string>   speciesList,
				std::vector<int>           speciesCount);
};
#endif