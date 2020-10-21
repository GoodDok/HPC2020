#include <cstring>
#include <stdexcept>
#include <cstdlib>
#include "matrix.h"
#include <omp.h>

using namespace std;

ostream &operator<<(ostream &strm, const Matrix2D &mat) {
  for (int i = 0; i < mat.n; ++i) {
    for (int j = 0; j < mat.m; ++j) {
      strm << mat.get(i, j) << " ";
    }
    strm << endl;
  }
  return strm;
}

Matrix2D Matrix2D::operator-(const Matrix2D &other) const {
  Matrix2D result = Matrix2D(n, m);
  for (int i = 0; i < other.n; ++i) {
    for (int j = 0; j < other.m; ++j) {
      result.set(i, j, get(i, j) - other.get(i, j));
    }
  }
  return result;
}

Matrix2D Matrix2D::operator*(const Matrix2D &other) const {
  if (m != other.n) {
    throw logic_error("Matrices sizes don't match!");
  }
  int newN = n;
  int newM = other.m;
  double *newMatrix = (double *) calloc(newN * newM, sizeof(double));
 
  if (1 == 0) {
    // use cache-optimized version
    for (int k = 0; k < m; ++k) {
      for (int i = 0; i < newN; ++i) {
        int first_idx = i * m + k;
        double *pnt1 = newMatrix + (newM * i);
        double *pnt2 = other.matrix + k * other.m;
        for (int j = 0; j < newM; ++j) {
          *pnt1 += matrix[first_idx] * *pnt2;
          ++pnt1;
          ++pnt2;
        }
      }
    }
  } else {
    // use parallelizable version
    for (int i = 0; i < newN; ++i) { 
      for (int j = 0; j < newM; ++j) {
        double val = 0;
#pragma omp parallel for reduction(+:val)
        for (int k = 0; k < m; ++k) {
          val += matrix[i * m + k] * other.matrix[k * other.m + j];
        }
        newMatrix[i * newM + j] = val;
      }
    } 
  }
  return Matrix2D(newN, newM, newMatrix);
}

Matrix2D Matrix2D::operator^(int pow) {
  if (pow <= 0) {
    throw logic_error("Operation not implemented!");
  }
  if (pow == 1) {
    return *this;
  } else if (pow % 2 == 1) {
     return *this * (*this^(pow - 1));
  } else {
     return (*this * *this)^(pow / 2);
  }
}

double Matrix2D::get(int i, int j) const {
  return matrix[i * m + j];
}
Matrix2D::Matrix2D(int n, int m) : n(n), m(m) {
  matrix = (double *) calloc(n * m, sizeof(double));
}

Matrix2D::Matrix2D(const Matrix2D &obj) {
  n = obj.n;
  m = obj.m;
  matrix = (double *) calloc(n * m, sizeof(double));
  memcpy(matrix, obj.matrix, n * m * sizeof(double));
}

Matrix2D& Matrix2D::operator=(const Matrix2D &obj) {
  if (this == &obj) {
     return *this;
  }
  n = obj.n;
  m = obj.m;
  matrix = (double *) calloc(n * m, sizeof(double));
  memcpy(matrix, obj.matrix, n * m * sizeof(double));
  return *this;
}

void Matrix2D::set(int i, int j, double val) {
  matrix[i * m + j] = val;
}

double Matrix2D::get_total_sum() const {
  double result = 0;
  for (int i = 0; i < n * m; i++) {
    result += matrix[i];
  }
  return result;
}
Matrix2D::~Matrix2D() {
  free(matrix);
}
