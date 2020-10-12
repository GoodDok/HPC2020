
#include <iostream>
using namespace std;

class Matrix2D {
 private:
  int n, m;
  double *matrix;

  Matrix2D(int n, int m, double *matrix) : n(n), m(m), matrix(matrix) {}
  friend std::ostream &operator<<(std::ostream &, const Matrix2D &);

 public:
  Matrix2D(int n, int m);

  double get(int i, int j) const;

  virtual ~Matrix2D();

  Matrix2D operator*(const Matrix2D &other) const;

  Matrix2D operator^(const Matrix2D &other) const;
};