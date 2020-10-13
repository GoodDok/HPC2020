#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *seeds;

void seed_threads(int *seeds) {
  int my_thread_id;
  unsigned int seed;
  my_thread_id = omp_get_thread_num();

  //create seed on thread using current time
  seed = (unsigned) time(NULL);

  //munge the seed using our thread number so that each thread has its
  //own unique seed, therefore ensuring it will generate a different set of numbers
  seeds[my_thread_id] = (seed & 0xFFFFFFF0) | (my_thread_id + 1);

  printf("Thread %d has seed %u\n", my_thread_id, seeds[my_thread_id]);
}

size_t count_random_point_hits(size_t N) {
  size_t threads_num;
  size_t hits = 0;
#pragma omp parallel default(none) shared(N, threads_num, seeds) reduction(+:hits)
  {
#pragma omp single
    {
      threads_num = omp_get_num_threads();
      seeds = (int *) malloc(sizeof(size_t) * threads_num);
    }
    seed_threads(seeds);
    int tid = omp_get_thread_num();
    int seed = seeds[tid];
    srand(seed);

#pragma omp for
    for (int i = 0; i < N; ++i) {
      double x = (double) rand_r(&seed) / RAND_MAX;
      double y = (double) rand_r(&seed) / RAND_MAX;
      if (x * x + y * y < 1) {
        ++hits;
      }
    }
  }
  free(seeds);
  return hits;
}

int main(int argc, char *argv[]) {
  size_t N = atoi(argv[1]);
  size_t hits = count_random_point_hits(N);
  printf("hits = %lu\n", hits);
  printf("approximated pi = %f\n", (double) hits * 4 / N);

  return 0;
}