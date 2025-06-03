#ifndef __MATH_LINALG_HPP__
#define __MATH_LINALG_HPP__

#include <vector>
#include <tuple>
#include <initializer_list>

namespace linalg {

class ivector;
class imatrix;
class fvector;
class fmatrix;

class ivector {
    public:
        ivector (void);
        ivector (int dim);
        ivector (std::initializer_list<int> list);
        ivector (const ivector & vec);

        int & operator [] (int idx);
        const int & operator [] (int idx) const;

        ivector & operator = (const ivector & vec);
        ivector & operator += (const ivector & vec);
        ivector & operator -= (const ivector & vec);
        ivector & operator *= (int scl);
        ivector & operator /= (int scl);

        friend bool operator == (const ivector & vec1, const ivector & vec2);
        friend bool operator != (const ivector & vec1, const ivector & vec2);
        friend bool operator >= (const ivector & vec1, const ivector & vec2);
        friend bool operator <= (const ivector & vec1, const ivector & vec2);
        friend bool operator > (const ivector & vec1, const ivector & vec2);
        friend bool operator < (const ivector & vec1, const ivector & vec2);

        friend ivector operator + (const ivector & vec);
        friend ivector operator - (const ivector & vec);

        friend ivector operator + (const ivector & vec1, const ivector & vec2);
        friend ivector operator - (const ivector & vec1, const ivector & vec2);
        friend ivector operator * (const ivector & vec, int scl);
        friend ivector operator * (int scl, const ivector & vec);
        friend ivector operator / (const ivector & vec, int scl);

        friend int operator * (const ivector & vec1, const ivector & vec2);
        friend ivector operator * (const ivector & vec, const imatrix & mat);
        friend ivector operator * (const imatrix & mat, const ivector & vec);
        friend imatrix operator * (const imatrix & mat1, const imatrix & mat2);
        friend imatrix operator ^ (const ivector & vec1, const ivector & vec2);

        friend int dim (const ivector & vec);
        friend int rows (const imatrix & mat);
        friend int cols (const imatrix & mat);
        friend int dim (const fvector & vec);
        friend int rows (const fmatrix & mat);
        friend int cols (const fmatrix & mat);

        friend void resize (ivector & vec, int dim);
        friend void resize (imatrix & mat, int rows, int cols);
        friend void resize (fvector & vec, int dim);
        friend void resize (fmatrix & mat, int rows, int cols);

        friend bool null (const ivector & vec);
        friend bool null (const imatrix & mat);
        friend bool null (const fvector & vec);
        friend bool null (const fmatrix & mat);

        friend bool sym (const fmatrix & mat);
        friend bool sym_pdf (const fmatrix & mat);
        friend bool sym_ndf (const fmatrix & mat);
        friend bool sym_psdf (const fmatrix & mat);
        friend bool sym_nsdf (const fmatrix & mat);

        friend double norm (const fvector & vec);
        friend double trac (const fmatrix & mat);
        friend double det (const fmatrix & mat);
        friend fmatrix tran (const fmatrix & mat);
        friend fmatrix inv (const fmatrix & mat);
        friend std::tuple<fvector, fmatrix> eig (const fmatrix & mat);

    private:
        int dim;
        std::vector<int> data;
};

class imatrix {
    public:
        imatrix (void);
        imatrix (int rows, int cols);
        imatrix (std::initializer_list<std::vector<int>> list);
        imatrix (const imatrix & mat);

        std::vector<int> & operator [] (int idx);
        const std::vector<int> & operator [] (int idx) const;

        imatrix & operator = (const imatrix & mat);
        imatrix & operator += (const imatrix & mat);
        imatrix & operator -= (const imatrix & mat);
        imatrix & operator *= (int scl);
        imatrix & operator /= (int scl);

        friend bool operator == (const imatrix & mat1, const imatrix & mat2);
        friend bool operator != (const imatrix & mat1, const imatrix & mat2);
        friend bool operator >= (const imatrix & mat1, const imatrix & mat2);
        friend bool operator <= (const imatrix & mat1, const imatrix & mat2);
        friend bool operator > (const imatrix & mat1, const imatrix & mat2);
        friend bool operator < (const imatrix & mat1, const imatrix & mat2);

        friend imatrix operator + (const imatrix & mat);
        friend imatrix operator - (const imatrix & mat);

        friend imatrix operator + (const imatrix & mat1, const imatrix & mat2);
        friend imatrix operator - (const imatrix & mat1, const imatrix & mat2);
        friend imatrix operator * (const imatrix & mat, int scl);
        friend imatrix operator * (int scl, const imatrix & mat);
        friend imatrix operator / (const imatrix & mat, int scl);

        friend int operator * (const ivector & vec1, const ivector & vec2);
        friend ivector operator * (const ivector & vec, const imatrix & mat);
        friend ivector operator * (const imatrix & mat, const ivector & vec);
        friend imatrix operator * (const imatrix & mat1, const imatrix & mat2);
        friend imatrix operator ^ (const ivector & vec1, const ivector & vec2);

        friend int dim (const ivector & vec);
        friend int rows (const imatrix & mat);
        friend int cols (const imatrix & mat);
        friend int dim (const fvector & vec);
        friend int rows (const fmatrix & mat);
        friend int cols (const fmatrix & mat);

        friend void resize (ivector & vec, int dim);
        friend void resize (imatrix & mat, int rows, int cols);
        friend void resize (fvector & vec, int dim);
        friend void resize (fmatrix & mat, int rows, int cols);

        friend bool null (const ivector & vec);
        friend bool null (const imatrix & mat);
        friend bool null (const fvector & vec);
        friend bool null (const fmatrix & mat);

        friend bool sym (const fmatrix & mat);
        friend bool sym_pdf (const fmatrix & mat);
        friend bool sym_ndf (const fmatrix & mat);
        friend bool sym_psdf (const fmatrix & mat);
        friend bool sym_nsdf (const fmatrix & mat);

        friend double norm (const fvector & vec);
        friend double trac (const fmatrix & mat);
        friend double det (const fmatrix & mat);
        friend fmatrix tran (const fmatrix & mat);
        friend fmatrix inv (const fmatrix & mat);
        friend std::tuple<fvector, fmatrix> eig (const fmatrix & mat);

    private:
        int rows, cols;
        std::vector<std::vector<int>> data;
};

class fvector {
    public:
        fvector (void);
        fvector (int dim);
        fvector (std::initializer_list<double> list);
        fvector (const fvector & vec);

        double & operator [] (int idx);
        const double & operator [] (int idx) const;

        fvector & operator = (const fvector & vec);
        fvector & operator += (const fvector & vec);
        fvector & operator -= (const fvector & vec);
        fvector & operator *= (double scl);
        fvector & operator /= (double scl);

        friend bool operator == (const fvector & vec1, const fvector & vec2);
        friend bool operator != (const fvector & vec1, const fvector & vec2);
        friend bool operator >= (const fvector & vec1, const fvector & vec2);
        friend bool operator <= (const fvector & vec1, const fvector & vec2);
        friend bool operator > (const fvector & vec1, const fvector & vec2);
        friend bool operator < (const fvector & vec1, const fvector & vec2);

        friend fvector operator + (const fvector & vec);
        friend fvector operator - (const fvector & vec);

        friend fvector operator + (const fvector & vec1, const fvector & vec2);
        friend fvector operator - (const fvector & vec1, const fvector & vec2);
        friend fvector operator * (const fvector & vec, double scl);
        friend fvector operator * (double scl, const fvector & vec);
        friend fvector operator / (const fvector & vec, double scl);

        friend double operator * (const fvector & vec1, const fvector & vec2);
        friend fvector operator * (const fvector & vec, const fmatrix & mat);
        friend fvector operator * (const fmatrix & mat, const fvector & vec);
        friend fmatrix operator * (const fmatrix & mat1, const fmatrix & mat2);
        friend fmatrix operator ^ (const fvector & vec1, const fvector & vec2);

        friend int dim (const ivector & vec);
        friend int rows (const imatrix & mat);
        friend int cols (const imatrix & mat);
        friend int dim (const fvector & vec);
        friend int rows (const fmatrix & mat);
        friend int cols (const fmatrix & mat);

        friend void resize (ivector & vec, int dim);
        friend void resize (imatrix & mat, int rows, int cols);
        friend void resize (fvector & vec, int dim);
        friend void resize (fmatrix & mat, int rows, int cols);

        friend bool null (const ivector & vec);
        friend bool null (const imatrix & mat);
        friend bool null (const fvector & vec);
        friend bool null (const fmatrix & mat);

        friend bool sym (const fmatrix & mat);
        friend bool sym_pdf (const fmatrix & mat);
        friend bool sym_ndf (const fmatrix & mat);
        friend bool sym_psdf (const fmatrix & mat);
        friend bool sym_nsdf (const fmatrix & mat);

        friend double norm (const fvector & vec);
        friend double trac (const fmatrix & mat);
        friend double det (const fmatrix & mat);
        friend fmatrix tran (const fmatrix & mat);
        friend fmatrix inv (const fmatrix & mat);
        friend std::tuple<fvector, fmatrix> eig (const fmatrix & mat);

    private:
        int dim;
        std::vector<double> data;
};

class fmatrix {
    public:
        fmatrix (void);
        fmatrix (int rows, int cols);
        fmatrix (std::initializer_list<std::vector<double>> list);
        fmatrix (const fmatrix & mat);

        std::vector<double> & operator [] (int idx);
        const std::vector<double> & operator [] (int idx) const;

        fmatrix & operator = (const fmatrix & mat);
        fmatrix & operator += (const fmatrix & mat);
        fmatrix & operator -= (const fmatrix & mat);
        fmatrix & operator *= (double scl);
        fmatrix & operator /= (double scl);

        friend bool operator == (const fmatrix & mat1, const fmatrix & mat2);
        friend bool operator != (const fmatrix & mat1, const fmatrix & mat2);
        friend bool operator >= (const fmatrix & mat1, const fmatrix & mat2);
        friend bool operator <= (const fmatrix & mat1, const fmatrix & mat2);
        friend bool operator > (const fmatrix & mat1, const fmatrix & mat2);
        friend bool operator < (const fmatrix & mat1, const fmatrix & mat2);

        friend fmatrix operator + (const fmatrix & mat);
        friend fmatrix operator - (const fmatrix & mat);

        friend fmatrix operator + (const fmatrix & mat1, const fmatrix & mat2);
        friend fmatrix operator - (const fmatrix & mat1, const fmatrix & mat2);
        friend fmatrix operator * (const fmatrix & mat, double scl);
        friend fmatrix operator * (double scl, const fmatrix & mat);
        friend fmatrix operator / (const fmatrix & mat, double scl);

        friend double operator * (const fvector & vec1, const fvector & vec2);
        friend fvector operator * (const fvector & vec, const fmatrix & mat);
        friend fvector operator * (const fmatrix & mat, const fvector & vec);
        friend fmatrix operator * (const fmatrix & mat1, const fmatrix & mat2);
        friend fmatrix operator ^ (const fvector & vec1, const fvector & vec2);

        friend int dim (const ivector & vec);
        friend int rows (const imatrix & mat);
        friend int cols (const imatrix & mat);
        friend int dim (const fvector & vec);
        friend int rows (const fmatrix & mat);
        friend int cols (const fmatrix & mat);

        friend void resize (ivector & vec, int dim);
        friend void resize (imatrix & mat, int rows, int cols);
        friend void resize (fvector & vec, int dim);
        friend void resize (fmatrix & mat, int rows, int cols);

        friend bool null (const ivector & vec);
        friend bool null (const imatrix & mat);
        friend bool null (const fvector & vec);
        friend bool null (const fmatrix & mat);

        friend bool sym (const fmatrix & mat);
        friend bool sym_pdf (const fmatrix & mat);
        friend bool sym_ndf (const fmatrix & mat);
        friend bool sym_psdf (const fmatrix & mat);
        friend bool sym_nsdf (const fmatrix & mat);

        friend double norm (const fvector & vec);
        friend double trac (const fmatrix & mat);
        friend double det (const fmatrix & mat);
        friend fmatrix tran (const fmatrix & mat);
        friend fmatrix inv (const fmatrix & mat);
        friend std::tuple<fvector, fmatrix> eig (const fmatrix & mat);

    private:
        int rows, cols;
        std::vector<std::vector<double>> data;
};

bool operator == (const ivector & vec1, const ivector & vec2);
bool operator != (const ivector & vec1, const ivector & vec2);
bool operator >= (const ivector & vec1, const ivector & vec2);
bool operator <= (const ivector & vec1, const ivector & vec2);
bool operator > (const ivector & vec1, const ivector & vec2);
bool operator < (const ivector & vec1, const ivector & vec2);

bool operator == (const imatrix & mat1, const imatrix & mat2);
bool operator != (const imatrix & mat1, const imatrix & mat2);
bool operator >= (const imatrix & mat1, const imatrix & mat2);
bool operator <= (const imatrix & mat1, const imatrix & mat2);
bool operator > (const imatrix & mat1, const imatrix & mat2);
bool operator < (const imatrix & mat1, const imatrix & mat2);

bool operator == (const fvector & vec1, const fvector & vec2);
bool operator != (const fvector & vec1, const fvector & vec2);
bool operator >= (const fvector & vec1, const fvector & vec2);
bool operator <= (const fvector & vec1, const fvector & vec2);
bool operator > (const fvector & vec1, const fvector & vec2);
bool operator < (const fvector & vec1, const fvector & vec2);

bool operator == (const fmatrix & mat1, const fmatrix & mat2);
bool operator != (const fmatrix & mat1, const fmatrix & mat2);
bool operator >= (const fmatrix & mat1, const fmatrix & mat2);
bool operator <= (const fmatrix & mat1, const fmatrix & mat2);
bool operator > (const fmatrix & mat1, const fmatrix & mat2);
bool operator < (const fmatrix & mat1, const fmatrix & mat2);

ivector operator + (const ivector & vec);
ivector operator - (const ivector & vec);

imatrix operator + (const imatrix & mat);
imatrix operator - (const imatrix & mat);

fvector operator + (const fvector & vec);
fvector operator - (const fvector & vec);

fmatrix operator + (const fmatrix & mat);
fmatrix operator - (const fmatrix & mat);

ivector operator + (const ivector & vec1, const ivector & vec2);
ivector operator - (const ivector & vec1, const ivector & vec2);
ivector operator * (const ivector & vec, int scl);
ivector operator * (int scl, const ivector & vec);
ivector operator / (const ivector & vec, int scl);

imatrix operator + (const imatrix & mat1, const imatrix & mat2);
imatrix operator - (const imatrix & mat1, const imatrix & mat2);
imatrix operator * (const imatrix & mat, int scl);
imatrix operator * (int scl, const imatrix & mat);
imatrix operator / (const imatrix & mat, int scl);

fvector operator + (const fvector & vec1, const fvector & vec2);
fvector operator - (const fvector & vec1, const fvector & vec2);
fvector operator * (const fvector & vec, double scl);
fvector operator * (double scl, const fvector & vec);
fvector operator / (const fvector & vec, double scl);

fmatrix operator + (const fmatrix & mat1, const fmatrix & mat2);
fmatrix operator - (const fmatrix & mat1, const fmatrix & mat2);
fmatrix operator * (const fmatrix & mat, double scl);
fmatrix operator * (double scl, const fmatrix & mat);
fmatrix operator / (const fmatrix & mat, double scl);

int operator * (const ivector & vec1, const ivector & vec2);
ivector operator * (const ivector & vec, const imatrix & mat);
ivector operator * (const imatrix & mat, const ivector & vec);
imatrix operator * (const imatrix & mat1, const imatrix & mat2);
imatrix operator ^ (const ivector & vec1, const ivector & vec2);

double operator * (const fvector & vec1, const fvector & vec2);
fvector operator * (const fvector & vec, const fmatrix & mat);
fvector operator * (const fmatrix & mat, const fvector & vec);
fmatrix operator * (const fmatrix & mat1, const fmatrix & mat2);
fmatrix operator ^ (const fvector & vec1, const fvector & vec2);

int dim (const ivector & vec);
int rows (const imatrix & mat);
int cols (const imatrix & mat);
int dim (const fvector & vec);
int rows (const fmatrix & mat);
int cols (const fmatrix & mat);

void resize (ivector & vec, int dim);
void resize (imatrix & mat, int rows, int cols);
void resize (fvector & vec, int dim);
void resize (fmatrix & mat, int rows, int cols);

bool null (const ivector & vec);
bool null (const imatrix & mat);
bool null (const fvector & vec);
bool null (const fmatrix & mat);

bool sym (const fmatrix & mat);
bool sym_pdf (const fmatrix & mat);
bool sym_ndf (const fmatrix & mat);
bool sym_psdf (const fmatrix & mat);
bool sym_nsdf (const fmatrix & mat);

double norm (const fvector & vec);
double trac (const fmatrix & mat);
double det (const fmatrix & mat);
fmatrix tran (const fmatrix & mat);
fmatrix inv (const fmatrix & mat);
std::tuple<fvector, fmatrix> eig (const fmatrix & mat);

}

#endif
