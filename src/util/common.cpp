#include "common.h"

Errors::Errors()
{}

Errors::~Errors()
{}


/*------------------------------------------------------------------------
Kill program, recieves messages from all methods in class
--------------------------------------------------------------------------*/
void Errors::kill(const char *file, const char *subroutine, int line, const char *message)
{
    fprintf (EMESG, "%s::ERROR\n line %d -> %s", subroutine,line, message);
    exit (EXIT_FAILURE);
}

void Errors::warn(const char *file, const char *subroutine, int line, const char *message)
{
    fprintf (WMESG, "%s::WARNING\n line %d -> %s", subroutine,line, message);
}

void Errors::show_usage_info(const char *func_name)
{
    printf("===========================================================================\n");
    printf("                     %s v. %s\n",SOFT,VERS);
    printf(" Usage: %s [-i|-o|-x|-y|-e]\n",func_name);
    printf("     -e|--entry        from which entry in the logfile to extract the data\n");
    printf("                       args = 1 | 2 | 10 ...; default is the last entry\n");
    printf("     -h|--help         show this message\n");
    printf("     -i|--input        name of logfile, can be the standard log.lammps or redirected log data\n");
    printf("     -k|--keywords     show a complete list of the accepted search terms for '-x|-y'\n");
    printf("                       and their translation into LAMMPS nomenclature\n");
    printf("     -o|--output       name of file for writing the xy data \n");
    printf("     -x|--x-axis       keyword of the data to be considred as the x-axis\n");
    printf("     -y|--y-axis       keyword of the data to be considred as the y-axis\n");
    printf("\n");
    printf("examples....\n");
    printf("  %s -k\n",func_name);
    printf("  %s -i log.lammps -e 1 -x Step -y Temp -o step-temp.dat\n",func_name);
    printf("  %s --input log.lammps --output step-ind3.dat --x-axis step --y-axis IND[3]\n",func_name);
    printf("===========================================================================\n");
}




Parser::Parser()
{};

Parser::~Parser()
{};

char* 
Parser::str2char(std::string str)
{
    char *arg = new char[str.size() + 1];
    std::copy(str.begin(), str.end(), arg);
    arg[str.size()] = '\0';
    return arg;
}

int 
Parser::match_phrase (FILE * fp, char *phrase, char *line)
{
   int n;
   //char line[MAXLENGTH];
   n = strlen (phrase);
   rewind (fp);
   while (1) {
        if (fgets (line, MAX_STRING_LENGTH, fp) == NULL) {
            printf ("\nUnable to find phrase %s.\n", phrase);
            return 0;
        }
        if (strncasecmp (phrase, line, n) == 0)
           return 1;
    }
}

/*------------------------------------------------------------------------
    Count arguments in "thermo_style" line
--------------------------------------------------------------------------*/
int 
Parser::count_words(std::string list)
{
	int count =0;
    std::istringstream iss(list);
    std::string nth;
    for(int ii=0;ii<20;ii++){
        nth.clear();
        iss >> nth;
        if(nth.empty()) break;
        else {
            count++;
        }
    }
    return count;
}


/*------------------------------------------------------------------------
    Search "thermo_style" line for user keywords
--------------------------------------------------------------------------*/
int 
Parser::find_rec(std::string list, std::string pattern, int& index)
{
    index = -1;
    std::istringstream iss(list);
    std::string lword, word;
    
    for(int ii=0;ii<20;ii++){
        word.clear();
        iss >> word;
        if(word == lword) {index = ii;return 1;}
    }
    return 0;
}

char*
Parser::str_manip(char *str, char *insert, char delim)
{
    std::string name(str);
    unsigned substr = name.find_last_of(delim);
    std::string fname = name.substr(0,substr);
    std::string ext = name.substr(substr+1);
    std::string outNM = fname + insert + ext;
    char *outfile = new char[outNM.length()+1];
    std::strcpy(outfile,outNM.c_str());
    return outfile;
}

std::vector<std::string> 
Parser::split_list(std::string list, char delim)
{
    std::vector<std::string> internal;
    std::stringstream ss(list);
    std::string token;
    while (std::getline(ss, token, delim)) internal.push_back(token);
    return internal;
}

int 
Parser::get_substrings(std::string list, char d1, char d2, std::vector<std::string> &keys, std::vector<std::string> &vals)
{
    std::vector<std::string> temp1, temp2;
    temp1 = split_list(list, d1);
    if (temp1.empty()) return 0;
    for (int i=0;i<temp1.size();i++) {
        temp2 = split_list(temp1[i],d2);
        keys.push_back(temp2[0]);
        vals.push_back(temp2[1]);
    }
    return 1;
}
