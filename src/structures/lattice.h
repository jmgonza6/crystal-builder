/** 
  * \class LATTICE lattice.h "lattice.h"
  *
  *> \par Purpose:
  *  =============
  * \verbatim 
  *
  *  Base class for constructing an arbitrary crystal structure based 
  *  on the fractional coordinates of the basis atoms, lattice vectors
  *  and lattice parameters a,b,c.
  *
  * \endverbatim
  * \author Joseph M. Gonzalez
  *
  * \version 0.2
  *
  * \date Sep 13, 2015 19:16:20
  *
  * \b Contact \n    jmgonza6@mail.usf.edu
  *
  */

#ifndef LATTICE_H
#define LATTICE_H

#include "../util/mathns.h"
#include "../util/common.h"


class LATTICE {
    public:
        
        Memory                   *memory;       //!< pointer to memory allocation handler

        // atomic data
        double                   **positions;  //!< final set of coordinates to write (fractional or cartesian)
        double                   **coords;     //!< cartesian coordinates for data file
        double                   **basis;      //!< basis atoms in single unit cell

        // cell info
        double                   alat;         //!< a lattice constant
        double                   blat;         //!< b lattice constant
        double                   clat;         //!< c lattice constant
        double                   alpha;        //!< angle between `b & c`
        double                   beta;         //!< angle between `a & c`
        double                   gamma;        //!< angle between `a & b`
        std::vector<double>      a1;           //!< a lattice vector, along [100]
        std::vector<double>      a2;           //!< b lattice vector, somewhere along [110]
        std::vector<double>      a3;           //!< c lattice vector, somewhere along [111]

        // atomic and type accounting
        int                      *type_count;  //!< pointer to keep track of how many of each type, `type_count[0] -> type 1 ...`
        int                      *nt;          //!< pointer to keep track of how many unique types
        int                      *atom_type;   //!< pointer to hold all unique types
        int                      *type_list;   //!< type numbers
        int                      natom;        //!< total atoms created
        int                      apcell;       //!< number of atoms in a single unit cell
        int                      ntype;        //!< number of atoms and atomic types in a unit cell
        int                      nx;           //!< number of unit along a1
        int                      ny;           //!< number of unit along a2
        int                      nz;           //!< number of unit along a3
        int                      fractional;   //!< `1` if the output coordinates are fractional, `0` otherwise
        
        // formating
        char                     *fformat;     //!< output file format, lammps, vasp, dmol
        char                     *name;        //!< name of the Bravais lattice, Cubic, Tetragonal, Graphene ...
        char                     *lstyle;      //!< style of the Bravais lattice
        std::vector<std::string> species;      //!< vector containing unique element symbols
        std::vector<int>         elemCount;

        // std::vector<std::vector<int> >      nnlist;
        
        LATTICE();
        ~LATTICE();

        /** \brief Display all parameters set during the GUI session
          */
        void 
        show_info();

        /** \brief Convert the cartesian atomic coordinates to fractional before writing the data 
          * \param[in] fractional - \a 1 if fractional \a 0 otherwise
          * \returns 1 if successful
          */
        int 
        convert_coordinates(int fractional);


        /** \brief Start the build process using the parameters set during the GUI session
          */
        void 
        build_crystal();

        int
        build_nn_list();

    private:
        /** \brief Convert the cartesian coordinates to fractional before writing the data 
          * \param[in] a1 - major lattice vector, in cartesian coordinates
          * \param[in] a2 - lattice vector, in `xy plane`, in cartesian coordinates
          * \param[in] a3 - lattice vector, in `xyz plane`, in cartesian coordinates
          * \param[in] cartesian - 2D array containing the atomic postions 
          */
        void
        cart2fract(std::vector<double> a1, 
                   std::vector<double> a2, 
                   std::vector<double> a3, 
                   double **           cartesian);

        

        void
        nn_search();
        
};
#endif
