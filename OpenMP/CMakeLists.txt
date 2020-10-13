cmake_minimum_required(VERSION 3.16)
project(OpenMPProject)

find_package(OpenMP REQUIRED)

set(CMAKE_C_STANDARD 99)
add_compile_options(-fopenmp)
add_executable(omp_bugreduction omp_bugreduction.c)
add_executable(omp_bugparfor omp_bugparfor.c)
add_executable(omp_monte_carlo_pi omp_monte_carlo_pi.c)
target_link_libraries(omp_bugreduction PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(omp_bugparfor PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(omp_monte_carlo_pi PRIVATE OpenMP::OpenMP_CXX)