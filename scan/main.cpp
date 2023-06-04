#include <vector>
#include <math.h>
#include <stdlib.h>
#include <chrono>
#include <stdio.h>
#include <algorithm>

#include <omp.h>

#include "problem.h"
#include "util.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Incorrect arguments - only 1 should be provided\n");
        return -1;
    }

    srand(time(NULL));
    size_t problem_size = std::pow(2, atoi(argv[1]));
    std::vector<int32_t> problem(problem_size);

    omp_set_num_threads(omp_get_max_threads());
    #pragma omp for
    for (size_t j = 0; j < problem_size; ++j) {
        problem[j] = rand() % (10 * problem_size);
    }

    printf("Problem size: %ld\n", problem_size);

    auto serial_start = std::chrono::high_resolution_clock::now();
    auto serial_solution = scan_serial(problem);
    auto serial_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> serial_time = serial_end - serial_start;
    printf("Serial Execution time = %f ms\n", serial_time.count());

    // Pass to OMP here
    auto omp_start = std::chrono::high_resolution_clock::now();
    auto omp_solution = scan_omp(problem);
    auto omp_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> omp_time = omp_end - omp_start;
    double omp_speedup = serial_time.count() / omp_time.count();
    printf("OMP Execution time = %f ms, OMP Speedup = %lf\n", omp_time.count(), omp_speedup);
    printf("OMP Correctness %s\n", is_correct(problem, omp_solution) ? "true" : "false");

    // Pass to MPI here
    auto mpi_start = std::chrono::high_resolution_clock::now();
    auto mpi_solution = scan_mpi(problem);
    auto mpi_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> mpi_time = mpi_end - mpi_start;
    double mpi_speedup = serial_time.count() / mpi_time.count();
    printf("MPI Execution time = %f ms, MPI Speedup = %lf\n", mpi_time.count(), mpi_speedup);
    printf("MPI Correctness %s\n\n", is_correct(problem, mpi_solution) ? "true" : "false");

    return 0;
}