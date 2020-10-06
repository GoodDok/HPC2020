#include <cstring>
#include "matrix.h"
#include <cblas.h>

std::ostream &operator<<(std::ostream &strm, const Matrix2D &mat) {
  for (int i = 0; i < mat.n; ++i) {
    for (int j = 0; j < mat.m; ++j) {
      strm << mat.get(i, j) << " ";
    }
    strm << endl;
  }
  return strm;
}
Matrix2D Matrix2D::operator*(const Matrix2D &other) const {
  if (m != other.n) {
    throw logic_error("Matrices sizes don't match!");
  }
  int newN = n;
  int newM = other.m;
  auto *newMatrix = (double *) malloc(newN * newM * sizeof(double));
  memset(newMatrix, 0, sizeof(*newMatrix));
  for (int k = 0; k < m; ++k) {
    for (int i = 0; i < newN; ++i) {
      int first_idx = i * m + k;
      double *pnt1 = newMatrix + (newN * i);
      double *pnt2 = other.matrix + k * other.m;
      for (int j = 0; j < newM; ++j) {
        *pnt1 += matrix[first_idx] * *pnt2;
        ++pnt1;
        ++pnt2;
      }
    }
  }
  return Matrix2D(newN, newM, newMatrix);
}
Matrix2D Matrix2D::operator^(const Matrix2D &other) const {
  if (m != other.n) {
    throw logic_error("Matrices sizes don't match!");
  }
  int newN = n;
  int newM = other.m;
  auto *newMatrix = (double *) malloc(newN * newM * sizeof(double));
  memset(newMatrix, 0, sizeof(*newMatrix));
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
              n, m, other.m, 1.0, matrix, m, other.matrix, other.m, 0.0, newMatrix, other.m);
  return Matrix2D(newN, newM, newMatrix);
}
double Matrix2D::get(int i, int j) const {
  return matrix[i * m + j];
}
Matrix2D::Matrix2D(int n, int m) : n(n), m(m) {
  matrix = (double *) malloc(n * m * sizeof(double));
  // generate matrix by size provided
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      matrix[i * m + j] = 1.0 * rand() / RAND_MAX;
    }
  }
}
Matrix2D::~Matrix2D() {
  free(matrix);
}
