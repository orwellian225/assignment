#include <algorithm>
#include <stdio.h>
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <chrono>

#include <omp.h>

#include "util.h"

void bitonic_sort(std::vector<int32_t>& array, size_t low, size_t count, bool direction, size_t depth);
void bitonic_merge(std::vector<int32_t>& array, size_t low, size_t count, bool direction);

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Please enter the problem size as the only argument");
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

    auto start = std::chrono::high_resolution_clock::now();
        bitonic_sort(problem, 0, problem.size(), true, 0);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time = end - start;

    printf("Serial Execution time %f ms\n", time.count());
    printf("Serial Correctness: %s\n", is_correct(problem) ? "true" : "false");

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
        bitonic_sort(array, low, midpoint, true, depth + 1); // Create ascending sequence
        bitonic_sort(array, low + midpoint, midpoint, false, depth + 1); // Create descending sequence
        bitonic_merge(array, low, count, direction);
    }

}