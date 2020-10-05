#include "matrix.h"

#include <iostream>
#include <sys/time.h>
#include <ctime>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Expected exactly 1 parameter!" << endl;
    return 1;
  }
  int n = stoi(argv[1]);

  int iterations = 5;

  Matrix2D first = Matrix2D(n, n);
  Matrix2D second = Matrix2D(n, n);

  struct timeval start{}, end{};
  double r_time;
  double min_runtime = 1e20, max_runtime = 0, sum_runtime = 0;
  for (int i = 0; i < iterations; ++i) {
    gettimeofday(&start, nullptr);
    Matrix2D result = first * second;
    gettimeofday(&end, nullptr);
    r_time = (double) (end.tv_sec - start.tv_sec) + ((double) (end.tv_usec - start.tv_usec)) / 1000000;
    if (r_time < min_runtime) {
      min_runtime = r_time;
    }
    if (r_time > max_runtime) {
      max_runtime = r_time;
    }
    sum_runtime += r_time;
  }

  cout << "n = " << n << ", iterations = " << iterations << endl;
  cout << "Min calc time in seconds = " << min_runtime << endl;
  cout << "Avg calc time in seconds = " << (sum_runtime / iterations) << endl;
  cout << "Max calc time in seconds = " << max_runtime << endl;

  return 0;
}
