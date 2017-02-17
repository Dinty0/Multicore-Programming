/*
  matrix.h --

  Two dimensional matrices.
  
  The class is templated and should work with any reasonable numerical type.

  The class offers C++11 braces initialization. A C++11-compliant or
  C++0x-compliant compiler is therefore mandatory.

  University of Nantes, X8II070 "Multicore Programming"
  Laboratory Work 2

  Author: Frédéric Goualard <Frederic.Goualard@univ-nantes.fr>
  v. 1.1, 2013-02-11

  ChangeLog:
  v. 1.2: Removed forward instantiations to comply with new version of g++  
  v. 1.1: Modified the implementation of the Matrix constructors 
          to avoid using C++11 'super' call to default constructor, 
          which is a feature not supported by the gcc compiler available 
	  in lab. classrooms.
  v. 1.0: Initial version
 */
#ifndef __matrix_h__
#define __matrix_h__

#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <algorithm>
#include <stdexcept>

template <typename T>
class Matrix {
 public:
  // Creation of an uninitialized matrix with r rows and c columns
 Matrix(uint32_t r, uint32_t c) : nrows(r), ncols(c) {
    mat = new T[nrows*ncols];
  }

  /*
    Creation of a matrix with r rows and c columns. All cells are
    initialized to 'init'.
  */
 Matrix(uint32_t r, uint32_t c, T init) : nrows(r), ncols(c) {
    mat = new T[nrows*ncols];
    for (uint32_t i = 0; i < nrows; ++i) {
      uint32_t dep = i*ncols;
      for (uint32_t j = 0; j < nrows; ++j) {
	mat[dep+j] = init;
      }
    }
  }

  /* C++11 braces-initialization. Allows initialization as follows:
     Matrix<int> M { 3, 3,  // Size of the matrix
     {2,  4, 5, // Unidimensional array to initialized the
     1, -5, 8, // matrix
     6,  7, 9}}; 
  */

 Matrix(uint32_t r, uint32_t c, 
	std::initializer_list<T> V) : Matrix(r,c) {
    if (nrows*ncols != V.size()) {
      throw std::runtime_error("Wrong size for initializing Matrix");
    }  
    auto e = V.begin();
    for (uint32_t i = 0; i < nrows; ++i) {
      uint32_t idx1 = i*ncols;
      for (uint32_t j = 0; j < ncols; ++j) {
	mat[idx1 + j] = *e;
	++e;
      }
    }
  }
  
  // Copy constructor
 Matrix(const Matrix& M) : nrows(M.nrows), ncols(M.ncols) {
    mat = new T[nrows*ncols];
    std::copy(M.mat,M.mat+nrows*ncols,mat);
  }
  
  ~Matrix() {
    delete[] mat;
  }

  /* Assignement.
     It is possible to assign to a matrix a matrix of different size. In that
     case, the memory used by the right-hand object to store the matrix is 
     freed and a new block is allocated.
  */
  Matrix& operator=(const Matrix& M) {
    if ((M.nrows != nrows) || (M.ncols != ncols)) {
      delete[] mat;
      nrows = M.nrows;
      ncols = M.ncols;
      mat = new T[nrows*ncols];
    }
    std::copy(M.mat,M.mat+nrows*ncols,mat);
    return *this;
  }

  // Accessor returning an rvalue
  T& operator()(uint32_t i, uint32_t j) {
    return mat[i*ncols+j];
  }
  
  // Accessor returning an lvalue
  T operator()(uint32_t i, uint32_t j) const {
    return mat[i*ncols+j];
  }

  // Number of rows in the matrix
  uint32_t rows(void) const {
    return nrows;
  }

  // Number of columns in the matrix
  uint32_t cols(void) const {
    return ncols;
  }
 private:
  uint32_t nrows, ncols;
  T *mat;
};


template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& M)
{
  for (uint32_t i = 0; i < M.rows(); ++i) {
    for (uint32_t j = 0; j < M.cols(); ++j) {
      os << M(i,j) << " ";
    }
    os << "\n";
  }
  return os;
}


// Some predefined types
typedef Matrix<double> DMatrix;
typedef Matrix<int> IMatrix;


#endif // __matrix_h__