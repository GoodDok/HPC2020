#include <iostream>
using namespace std;

class Matrix2D {
 public:
  int n, m;
  double *matrix;

  friend ostream &operator<<(ostream &, const Matrix2D &);

  Matrix2D(int n, int m, double *matrix) : n(n), m(m), matrix(matrix) {}

  Matrix2D(int n, int m);

  double get(int i, int j) const;
  
  void set(int i, int j, double val);
  
  double get_total_sum() const;

  virtual ~Matrix2D();

  Matrix2D operator*(const Matrix2D &other) const;
  
  Matrix2D operator-(const Matrix2D &other) const;
  
  Matrix2D operator^(int pow);
  
  Matrix2D(const Matrix2D &obj);

  Matrix2D& operator=(const Matrix2D &obj);
};
