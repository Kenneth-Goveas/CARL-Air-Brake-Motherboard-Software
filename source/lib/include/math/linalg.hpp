#ifndef __MATH_LINALG_HPP__
#define __MATH_LINALG_HPP__

#include <vector>

namespace linalg {

class ivector;
class imatrix;
class fvector;
class fmatrix;

class ivector {
    public:
         ivector (void);
         ivector (int dim);
         ivector (const ivector & vec);
        ~ivector (void);

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

        friend void resize (ivector * vec, int dim);
        friend void resize (imatrix * mat, int rows, int cols);

        friend bool is_null (int scl);
        friend bool is_null (const ivector & vec);
        friend bool is_null (const imatrix & mat);

        friend bool is_finite (int scl);
        friend bool is_finite (const ivector & vec);
        friend bool is_finite (const imatrix & mat);

    private:
        int dim;
        std::vector<int> data;
};

class imatrix {
    public:
         imatrix (void);
         imatrix (int rows, int cols);
         imatrix (const imatrix & mat);
        ~imatrix (void);

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

        friend void resize (ivector * vec, int dim);
        friend void resize (imatrix * mat, int rows, int cols);

        friend bool is_null (int scl);
        friend bool is_null (const ivector & vec);
        friend bool is_null (const imatrix & mat);

        friend bool is_finite (int scl);
        friend bool is_finite (const ivector & vec);
        friend bool is_finite (const imatrix & mat);

    private:
        int rows, cols;
        std::vector<std::vector<int>> data;
};

class fvector {
    public:
         fvector (void);
         fvector (int dim);
         fvector (const fvector & vec);
        ~fvector (void);

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

        friend int dim (const fvector & vec);
        friend int rows (const fmatrix & mat);
        friend int cols (const fmatrix & mat);

        friend void resize (fvector * vec, int dim);
        friend void resize (fmatrix * mat, int rows, int cols);

        friend bool is_null (double scl);
        friend bool is_null (const fvector & vec);
        friend bool is_null (const fmatrix & mat);

        friend bool is_finite (double scl);
        friend bool is_finite (const fvector & vec);
        friend bool is_finite (const fmatrix & mat);

        friend void filter (double * scl);
        friend void filter (fvector * vec);
        friend void filter (fmatrix * mat);

        friend fvector zeros (int dim);
        friend fmatrix zeros (int rows, int cols);
        friend fvector ones (int dim);
        friend fmatrix ones (int rows, int cols);
        friend fvector ident (int dim, int idx);
        friend fmatrix ident (int dim);

        friend double norm (const fvector & vec);
        friend double trac (const fmatrix & mat);
        friend double det (const fmatrix & mat);
        friend fmatrix tran (const fmatrix & mat);
        friend fmatrix inv (const fmatrix & mat);

        friend bool is_sym (const fmatrix & mat);
        friend bool is_sym_pdf (const fmatrix & mat);
        friend bool is_sym_ndf (const fmatrix & mat);
        friend bool is_sym_psdf (const fmatrix & mat);
        friend bool is_sym_nsdf (const fmatrix & mat);

        friend void plu (
            const fmatrix & mat, fmatrix * matp, fmatrix * matl, fmatrix * matu
        );
        friend void qr (const fmatrix & mat, fmatrix * matq, fmatrix * matr);
        friend void rq (const fmatrix & mat, fmatrix * matr, fmatrix * matq);
        friend void ql (const fmatrix & mat, fmatrix * matq, fmatrix * matl);
        friend void lq (const fmatrix & mat, fmatrix * matl, fmatrix * matq);
        friend void qdq (const fmatrix & mat, fmatrix * matq, fmatrix * matd);

    private:
        int dim;
        std::vector<double> data;
};

class fmatrix {
    public:
         fmatrix (void);
         fmatrix (int rows, int cols);
         fmatrix (const fmatrix & mat);
        ~fmatrix (void);

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

        friend int dim (const fvector & vec);
        friend int rows (const fmatrix & mat);
        friend int cols (const fmatrix & mat);

        friend void resize (fvector * vec, int dim);
        friend void resize (fmatrix * mat, int rows, int cols);

        friend bool is_null (double scl);
        friend bool is_null (const fvector & vec);
        friend bool is_null (const fmatrix & mat);

        friend bool is_finite (double scl);
        friend bool is_finite (const fvector & vec);
        friend bool is_finite (const fmatrix & mat);

        friend void filter (double * scl);
        friend void filter (fvector * vec);
        friend void filter (fmatrix * mat);

        friend fvector zeros (int dim);
        friend fmatrix zeros (int rows, int cols);
        friend fvector ones (int dim);
        friend fmatrix ones (int rows, int cols);
        friend fvector ident (int dim, int idx);
        friend fmatrix ident (int dim);

        friend double norm (const fvector & vec);
        friend double trac (const fmatrix & mat);
        friend double det (const fmatrix & mat);
        friend fmatrix tran (const fmatrix & mat);
        friend fmatrix inv (const fmatrix & mat);

        friend bool is_sym (const fmatrix & mat);
        friend bool is_sym_pdf (const fmatrix & mat);
        friend bool is_sym_ndf (const fmatrix & mat);
        friend bool is_sym_psdf (const fmatrix & mat);
        friend bool is_sym_nsdf (const fmatrix & mat);

        friend void plu (
            const fmatrix & mat, fmatrix * matp, fmatrix * matl, fmatrix * matu
        );
        friend void qr (const fmatrix & mat, fmatrix * matq, fmatrix * matr);
        friend void rq (const fmatrix & mat, fmatrix * matr, fmatrix * matq);
        friend void ql (const fmatrix & mat, fmatrix * matq, fmatrix * matl);
        friend void lq (const fmatrix & mat, fmatrix * matl, fmatrix * matq);
        friend void qdq (const fmatrix & mat, fmatrix * matq, fmatrix * matd);

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

void init (int tol_ord, double tol_abs, double tol_rel);

int dim (const ivector & vec);
int rows (const imatrix & mat);
int cols (const imatrix & mat);

int dim (const fvector & vec);
int rows (const fmatrix & mat);
int cols (const fmatrix & mat);

void resize (ivector * vec, int dim);
void resize (imatrix * mat, int rows, int cols);

void resize (fvector * vec, int dim);
void resize (fmatrix * mat, int rows, int cols);

bool is_null (int scl);
bool is_null (const ivector & vec);
bool is_null (const imatrix & mat);

bool is_null (double scl);
bool is_null (const fvector & vec);
bool is_null (const fmatrix & mat);

bool is_finite (int scl);
bool is_finite (const ivector & vec);
bool is_finite (const imatrix & mat);

bool is_finite (double scl);
bool is_finite (const fvector & vec);
bool is_finite (const fmatrix & mat);

bool is_eq (double scl1, double scl2);
bool is_neq (double scl1, double scl2);
bool is_geq (double scl1, double scl2);
bool is_leq (double scl1, double scl2);
bool is_gneq (double scl1, double scl2);
bool is_lneq (double scl1, double scl2);

void filter (double * scl);
void filter (fvector * vec);
void filter (fmatrix * mat);

fvector zeros (int dim);
fmatrix zeros (int rows, int cols);
fvector ones (int dim);
fmatrix ones (int rows, int cols);
fvector ident (int dim, int idx);
fmatrix ident (int dim);

double norm (const fvector & vec);
double trac (const fmatrix & mat);
double det (const fmatrix & mat);
fmatrix tran (const fmatrix & mat);
fmatrix inv (const fmatrix & mat);

bool is_sym (const fmatrix & mat);
bool is_sym_pdf (const fmatrix & mat);
bool is_sym_ndf (const fmatrix & mat);
bool is_sym_psdf (const fmatrix & mat);
bool is_sym_nsdf (const fmatrix & mat);

void plu (const fmatrix & mat, fmatrix * matp, fmatrix * matl, fmatrix * matu);
void qr (const fmatrix & mat, fmatrix * matq, fmatrix * matr);
void rq (const fmatrix & mat, fmatrix * matr, fmatrix * matq);
void ql (const fmatrix & mat, fmatrix * matq, fmatrix * matl);
void lq (const fmatrix & mat, fmatrix * matl, fmatrix * matq);
void qdq (const fmatrix & mat, fmatrix * matq, fmatrix * matd);

}

#endif
