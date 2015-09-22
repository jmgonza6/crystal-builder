/** 
  * \file common.h 
  *
  * \b Purpose:
  *  =============
  \verbatim 
  /********************************************************************************\
  /  A place to define application wide constants, classes, and structures         \
  /  Classes declared within this header:  Errors, Memory, Parser                  \
  /********************************************************************************\
  \endverbatim
  *
  * \author Joseph M. Gonzalez
  *
  * \version 0.2
  *
  * \date Sep 13, 2015 19:16:20
  *
  * \b Contact \n    jmgonza6@mail.usf.edu
  *
  */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <gtk/gtk.h>

#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <ctime>
#include <cmath>

// STL
#include <list>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>


#define SOFT "crystal builder"  //!< application name
#define VERS "3.5.1"            //!< current version of the application

#define MAX_LOG_ENTRIES 30
#define MAX_THERMO_LIST 50
#define	MAX_STRING_LENGTH 256   //!< max length of a string for allocating \b `char[]` types


#define PI 3.14159265359        //!< long form of PI
#define R2D 180/3.14159265359   //!< convert radians to degrees
#define D2R 3.14159265359/180   //!< convert degrees to radians



// error codes
#define FERR __FILE__           //!< file where error happened
#define LERR __LINE__           //!< line where error happened
#define BERR 101                //!< basic error, print usage
#define RERR 404		        //!< reading error
#define WERR 303		        //!< writing error.  most likely fname==NULL
#define IERR 909                //!< initialization error
#define AERR 666                //!< unknown argument error
#define MERR 999                //!< memory error

#define FDEBUG stderr           //!< where to print debug messages
#define SMESG stdout
#define WMESG stdout
#define EMESG stderr


// define how much info is printed to the terminal

// print global debugging information throughout the application
#ifdef DEBUGMSG
    #define DMSG 1
#else
    #define DMSG 0
#endif

// print messages/status updates during the gui session
#ifdef GUIMESG
    #define GMSG 1
#else
    #define GMSG 0
#endif

// print messages/status updates during the rendering process
#ifdef RENDERMSG
    #define RMSG 1
#else
    #define RMSG 0
#endif



/** \struct PeriodicTable
  * \brief A structure containing strings representing up to element 110 
  * and their associated masses in AMU
  */
struct PeriodicTable{
    std::string elements[110];
    double amu[110];
};


/** 
  * \class Errors common.h "common.h"
  *
  * \b Purpose:
  *  =============
  \verbatim 
  /*****************************************************************************\
  /  Class to process different types of warnings, errors and usage info        \
  /                                                                             \
  /*****************************************************************************\
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
// version 0.2 of the error class
class Errors {
	public:
		Errors();
		~Errors();
		/** \brief Throw a fatal error and kill the entire run 
		  * \param file - file in which error occured
		  * \param subroutine - subroutine throwing the error
		  * \param line - line on which the error occured
		  * \param message - message to the user with the details of the error
		  */
        void 
        kill(const char *file, const char *subroutine, int line, const char *message);

        /** \brief Throw a warning, print a message and continue on
		  * \param file - file in which warning occured
		  * \param subroutine - subroutine throwing the warning	
		  * \param line - line on which the warning occured
		  * \param message - message to the user with the details of the warning
		  */
        void 
        warn(const char *file, const char *subroutine, int line, const char *message);

        /** \brief Show a list of the possible parameters and operational modes of the program
		  * \param funcName - name of the calling function
		  */
		void 
		show_usage_info(const char *funcName);

};

/**
 * \class MEMORY memory.h "memory.h"
 * \b Purpose:
  *  =============
  \verbatim 
  /********************************************************************************\
  /  Template based memory managment class for use in allocating/deallocating      \
  /  blocks of memory using new[]                                                  \
  /                                                                                \
  /********************************************************************************\
  \endverbatim
 *
 * \author Joseph M. Gonzalez
 *
 * \version 0.1
 *
 * \date May 28, 2015 19:16:20
 *
 * \b Contact \n    jmgonza6@mail.usf.edu
 *
 */
class Memory {
    // class ERRORS *errors;
    public:
        
        Memory()
        {};
        ~Memory()
        {};

        // * \brief Safe method of deleting memory allocations 
        //   * \param ptr - 1D array, can be any type
          
        // void safe_delete(void *ptr);

        // 1D array/vector functions
        /** \brief Template to create a 1D array using \b `new` 
          * \tparam array - Address of array to be allocated
          * \tparam length - Length of array 
          * \returns 1D array of type \b `TYPE`
          */
        template <typename TYPE>
        TYPE *new_1d(TYPE *&array, int length)
        {
            int ii,jj;
            array = new TYPE[length];
            for(ii=0;ii<length;++ii) 
                array[ii] = 0.;
            return array;
        }

        /** \brief Safe method of deleting memory allocations 
          * \tparam array - 1D array created by new_1d or using \b `new` 
          */
        template <typename TYPE>
        void destroy(TYPE *array)
        {
            if (!array) return;
            delete array;
        }



        // 2D array/matrix functions
        /** \brief Template to create a 2D array using \b `new` 
          * \tparam array - Address of array to be allocated
          * \tparam rows - Number of rows in array
          * \tparam columns - Number of columns in array 
          * \returns 2D array of type \b `TYPE`
          */
        template <typename TYPE>
        TYPE **new_2d(TYPE **&array, int rows, int columns)
        {
            int ii,jj;
            array = new TYPE*[rows*columns];
            for(ii=0;ii<rows*columns;++ii) 
                array[ii] = new TYPE[columns];

            for(ii=0; ii<rows;ii++)
                for(jj=0; jj<columns; jj++)
                    array[ii][jj] = 0.;
            return array;
        }

        /** \brief Safe method of deleting memory allocations 
          * \tparam array - 2D array created by new_2d or using \b `new` 
          */
        template <typename TYPE>
        void destroy(TYPE **array)
        {
            if (!array) return;
            delete array[0];
            delete array;
            // safe_delete(array[0]);
            // safe_delete(array);
        }


        // 2D arrays as 1D vectors, built for speed!
        /** \brief Template to create a 2D array and mapped as a 1D array using \b `new` 
          * \tparam array - Address of array to be allocated
          * \tparam length - Length of array 
          * \returns 2D array mapped to 1D array of type \b `TYPE`
          */
        template <typename TYPE>
        TYPE *flat_2d(TYPE **&array, int length)
        {
            int ii,jj;
            array = new TYPE*[length];
            for(ii=0;ii<length;++ii) 
                array[ii] = 0.;
            return array;
        }

        template<typename retVal>
        retVal 
        *new_flat(int length)
        {
          int ii;
            retVal *array = new retVal[length];
            for(ii=0;ii<length;++ii) 
                array[ii] = 0.;
            return array;
        }
};


/** 
  * \class Parser common.h "common.h"
  *
  * \b Purpose:
  *  =============
  \verbatim 
  /*****************************************************************************\
  /  Class to process different types of strings, mainly conversion and search  \
  /  functions.                                                                 \
  /                                                                             \
  /*****************************************************************************\
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
class Parser {

    public:
        Parser();
        ~Parser();

        /** \brief Template to convert a \b `std::string` type to a numeric type
          * \tparam numstr - Address of the string to convert
          * \returns a number of type \b `retVal`
          */
        template<typename retVal>
        retVal 
        str2num(const std::string& numstr)
        {
          retVal number;
          std::stringstream stream(numstr);
          stream >> number;
          return number;
        }

        /** \brief Converts a \b `std::string` type to \b `char *`
          * \param[in] str - the string to be converted
          * \returns \b `char *` if successfull, \b `NULL` otherwise
          */
        char* 
        str2char(std::string str);

        /** \brief Search for a specific pattern in a text file
          * \param[in] fptr - Pointer to the file
          * \param[in] phrase - Specific pattern to match
          * \param[out] line - Entire line containing the pattern, simillar to `grep`
          * \returns \b `1` if successful, \b `0` otherwise
          */
        int 
        match_phrase(FILE   *fptr, 
                     char   *phrase, 
                     char   *line);

        /** \brief Count the number of words in a string
          * \param[in] list - strings to be parsed based on whitespace
          * \returns number of words
          */
        int
        count_words(std::string list);

        /** \brief Find a word in a list of words separated by whitespace
          * \param[in] list - string of words to be parsed
          * \param[in] pattern - pattern to match in `list`
          * \param[out] index - strings to be parsed based on whitespace
          * \returns \b `1` if successful, \b `0` otherwise. 
          */
        int
        find_rec(std::string list, 
                 std::string pattern, 
                 int         &index);


        /** \brief Parse a string of words based on a delimeter
          * \param[in] list - string of words to be parsed
          * \param[in] delim - delimeter to use for splitting
          * \returns a \b `std::vector<std::string>` type containing the words found in `list`
          */
        std::vector<std::string> 
        split_list(std::string list, 
                   char        delim);


        /** \brief Insert a word based on a delimeter
          * \param[in] str - string to have a word inserted
          * \param[in] insert - word to insert in `str`
          * \returns a \b `char*` type containing compound word
          */
        char*
        str_manip(char *str, 
                  char *insert, 
                  char delim);


        /** \brief Parse a string of words based on two delimeters
          * \param[in] list - string of words to be parsed
          * \param[in] d1 - fisrt delimeter to use for splitting
          * \param[in] d2 - second delimeter to use for splitting the result of `d1`
          * \param[in] keys - keywords to search for in `list`
          * \param[in] vals - values associated with `keys` in `list`
          * \returns \b `1` if successful, \b `0` otherwise.
          */
        int
        scan_list(std::string              list,
                  char                     d1, 
                  char                     d2, 
                  std::vector<std::string> &keys,  
                  std::vector<std::string> &vals);

};



#endif