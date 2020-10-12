cmake_minimum_required(VERSION 3.16)
project(OpenMP C)

set(CMAKE_C_STANDARD 99)
add_compile_options(-fopenmp)
add_executable(omp_bugreduction omp_bugreduction.c)
add_executable(omp_bugparfor omp_bugparfor.c)