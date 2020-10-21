#include "pagerank.h"
#include <cmath>
#include <cassert>
#include <omp.h>

using namespace std;


Matrix2D prepare_matrix(const Matrix2D &matrix, double damp) {
  Matrix2D norm = matrix;
  for (int j = 0; j < norm.m; ++j) {
    double column_sum = 0;
#pragma omp parallel for reduction(+:column_sum)
    for (int i = 0; i < norm.n; ++i) {
      column_sum += norm.get(i, j);
    }
#pragma omp parallel for
    for (int i = 0; i < norm.n; ++i) {
      norm.set(i, j, (1 - damp) / norm.m + damp * norm.get(i, j) / column_sum);
    }
  }
  return norm;
}

double get_vector_norm(const Matrix2D &v) {
  double sum = 0;
//#pragma omp parallel for reduction(+:sum)
  for (int i = 0; i < v.n; ++i) {
     double val = v.get(i, 0);
     sum += val * val;
  }
  return sqrt(sum);
}

Matrix2D normalize_vector(const Matrix2D& v) {
  double norm = get_vector_norm(v);
  Matrix2D norm_vec = Matrix2D(v.n, v.m);
  for (int i = 0; i < v.n; ++i) {
     norm_vec.set(i, 0, v.get(i, 0) / norm);
  }
  return norm_vec;
}

Matrix2D find_pagerank(const Matrix2D &a) {
  double eps = 1e-10;
  Matrix2D prep_a = prepare_matrix(a, 0.9);
  Matrix2D vec = Matrix2D(prep_a.n, 1);
  for (int i=0; i<prep_a.n; ++i) {
    vec.set(i, 0, 1);
  }
  double diff = 1.0;
  int iter_limit = 10000;
  while (diff >= eps && iter_limit > 0) {
    --iter_limit;
    Matrix2D prev_vec = vec;
    vec = prep_a * vec;
    vec = normalize_vector(vec);
    diff = get_vector_norm(prev_vec - vec);
  }
  assert(iter_limit > 0);
  return vec;
}
