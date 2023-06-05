#include <algorithm>
#include <stdio.h>
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <chrono>

#include <omp.h>

#include "util.h"

#define pow2(x) std::pow(2, x)

void bitonic_sort(std::vector<int32_t>& array, size_t low, size_t count, bool direction, size_t depth);
void bitonic_merge(std::vector<int32_t>& array, size_t low, size_t count, bool direction);

static size_t NUM_THREADS;

int main(int argc, char* argv[]) {

    size_t problem_size = argc >= 2 ? pow2(atoi(argv[1])) : pow2(2);
    NUM_THREADS = argc >= 3 ? atoi(argv[2]) : omp_get_max_threads();

    srand(time(NULL));
    std::vector<int32_t> problem(problem_size);

    // omp_set_num_threads(omp_get_max_threads());
    #pragma omp parallel for
    for (size_t j = 0; j < problem_size; ++j) {
        problem[j] = rand() % (10 * problem_size);
    }

    printf("Problem size: %ld\n", problem_size);

    omp_set_num_threads(NUM_THREADS);
    auto start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel
    {
        #pragma omp single
        bitonic_sort(problem, 0, problem.size(), true, 0);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time = end - start;

    printf("OMP Execution time %f ms\n", time.count());
    printf("OMP Correctness: %s\n", is_correct(problem) ? "true" : "false");

    return 0;
}

void bitonic_merge(std::vector<int32_t>& array, size_t low, size_t count, bool direction) {

    if (count > 1) {
        size_t midpoint = count / 2;

        for (size_t i = low; i < low + midpoint; ++i) {
            if (direction == array[i] > array[midpoint + i]) {
                int32_t temp = array[i];
                array[i] = array[midpoint + i];
                array[midpoint + i] = temp;
            }
        }
        bitonic_merge(array, low, midpoint, direction);
        bitonic_merge(array, low + midpoint, midpoint, direction);
    }
}

void bitonic_sort(std::vector<int32_t>& array, size_t low, size_t count, bool direction, size_t depth) {
    if (count > 1) {
        size_t midpoint = count / 2;
        if (pow2(depth) < NUM_THREADS) {
            #pragma omp parallel sections 
            {
                #pragma omp section
                bitonic_sort(array, low, midpoint, true, depth + 1); // Create ascending sequence
                #pragma omp section
                bitonic_sort(array, low + midpoint, midpoint, false, depth + 1); // Create descending sequence
            }
        } else {
            bitonic_sort(array, low, midpoint, true, depth + 1); // Create ascending sequence
            bitonic_sort(array, low + midpoint, midpoint, false, depth + 1); // Create descending sequence
        }

        bitonic_merge(array, low, count, direction);
    }

}