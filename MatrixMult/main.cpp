#include "matrix.h"

#include <iostream>
#include <functional>
#include <sys/time.h>
#include <ctime>

using namespace std;

struct timing_result {
  double min, avg, max;
};

std::ostream &operator<<(std::ostream &strm, const timing_result result) {
  strm << "Min calc time in seconds = " << result.min << endl;
  strm << "Avg calc time in seconds = " << result.avg << endl;
  strm << "Max calc time in seconds = " << result.max << endl;
  return strm;
}

timing_result measure_timing(const function<void(void)> &f, int iterations) {
  struct timeval start{}, end{};
  double r_time;
  double min_runtime = 1e20, max_runtime = 0, sum_runtime = 0;
  for (int i = 0; i < iterations; ++i) {
    gettimeofday(&start, nullptr);
    f();
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
  return timing_result{min_runtime, sum_runtime / iterations, max_runtime};
}

void measure_multiplication_by_size(int n, int iterations) {

  Matrix2D first = Matrix2D(n, n);
  Matrix2D second = Matrix2D(n, n);

  function<void(void)> custom_mult = [&first, &second]() { first * second; };
  auto timing = measure_timing(custom_mult, iterations);
  cout << "Custom implementation: " << endl << timing << endl;

  function<void(void)> blas_mult = [&first, &second]() { first ^ second; };
  timing = measure_timing(blas_mult, iterations);
  cout << "CBLAS implementation: " << endl << timing << endl;
}

int main(int argc, char *argv[]) {
  int iterations = 5;
  int curr_dim = 512;
  while (curr_dim < 5000) {
    cout << "n = " << curr_dim << ", iterations = " << iterations << endl;
    measure_multiplication_by_size(curr_dim, iterations);
    curr_dim <<= 1;
  }
  return 0;
}
