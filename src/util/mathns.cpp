// file: mathns.cpp
#include "mathns.h"

class Errors *merrors;
class Memory *memory;

/*------------------------------------------------------------
    LEVEL-1 OPERATIONS
--------------------------------------------------------------*/

double MATHNS::vector_mag(std::vector<double> v)
{
    int dim = v.size();
    if(!dim) 
        merrors->kill(FERR,"MATHNS::vector_mag()",LERR,"vector has 0 length!");

    double sum=0;
    for(int ii=0; ii<dim; ii++) 
        sum += v[ii]*v[ii]; 
    return sqrt( sum );
}

double MATHNS::vector_dot(std::vector<double> a, std::vector<double> b )
{
    int adim = a.size();
    int bdim = b.size();
    if( (!a.size()) || (!b.size()) )
        merrors->kill(FERR,"MATHNS::vector_dot()",LERR,"vector 'a' or 'b' has 0 length!");
    if( a.size() != b.size() )
        merrors->kill(FERR,"MATHNS::vector_dot()",LERR,"legnth(a) != length(b)!");
    
    double sum=0;
    for(int ii=0; ii<adim; ii++) 
        sum += a[ii]*b[ii]; 
    return sum;
}

double MATHNS::angle(std::vector<double> u, std::vector<double> v)
{
    double um = vector_mag(u);
    double vm = vector_mag(v);
    double dot = vector_dot(u,v);
    return acos(dot/(um*vm));
}

/*------------------------------------------------------------
    LEVEL-2 OPERATIONS
--------------------------------------------------------------*/

std::vector<double> MATHNS::vector_cross(std::vector<double> a, std::vector<double> b)
{
    if( (!a.size()) || (!b.size()) )
        merrors->kill(FERR,"MATHNS::vector_cross()",LERR,"vector 'a' or 'b' has 0 length!");
    std::vector<double> c(3);
    c[0] = a[1]*b[2] - a[2]*b[1];
    c[1] = a[2]*b[0] - a[0]*b[2];
    c[2] = a[0]*b[1] - a[1]*b[0];
    return c;
}

std::vector<double> MATHNS::mat_vec_mult(int lda, double **A, std::vector<double> X)
{
    
    if(lda!=X.size())
        merrors->kill(FERR,"MATHNS::mat_vec_mult()",LERR,"leading dimension of A != leading dimension of X!");
    std::vector<double> vout(lda);
    for(int ii=0; ii<lda; ii++) {
        vout[ii] = 0.;
        for(int kk=0; kk<lda; kk++)
            vout[ii] += A[ii][kk]*X[kk];
    }
    return vout;
}

std::vector<double> MATHNS::rotate_vector(std::vector<double> X, std::string axis, double angle)
{
    std::vector<double> rotated;
    double **rotMatrix;
    memory->new_2d(rotMatrix,3,3);
    if (axis=="x") {
        rotMatrix[0][0] = 1;  rotMatrix[0][1] = 0;              rotMatrix[0][2] =  0;
        rotMatrix[1][0] = 0;  rotMatrix[1][1] = cos(angle*D2R); rotMatrix[1][2] = -sin(angle*D2R);
        rotMatrix[2][0] = 0;  rotMatrix[2][1] = sin(angle*D2R); rotMatrix[2][2] =  cos(angle*D2R);
    } else if (axis=="y") {
        rotMatrix[0][0] = cos(angle*D2R);  rotMatrix[0][1] = 0; rotMatrix[0][2] = sin(angle*D2R);
        rotMatrix[1][0] = 0;               rotMatrix[1][1] = 1; rotMatrix[1][2] = 0;
        rotMatrix[2][0] = -sin(angle*D2R); rotMatrix[2][1] = 0; rotMatrix[2][2] = cos(angle*D2R);
    } else if (axis=="z") {
        rotMatrix[0][0] = cos(angle*D2R);  rotMatrix[0][1] = -sin(angle*D2R); rotMatrix[0][2] = 0;
        rotMatrix[1][0] = sin(angle*D2R);  rotMatrix[1][1] =  cos(angle*D2R); rotMatrix[1][2] = 0;
        rotMatrix[2][0] = 0;               rotMatrix[2][1] =  0;              rotMatrix[2][2] = 1;
    }

    rotated = mat_vec_mult(3,rotMatrix,X);

    memory->destroy(rotMatrix);
    return rotated;
}
