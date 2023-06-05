#include <algorithm>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include <omp.h>

#include "util.h"

std::vector<int32_t> scan(const std::vector<int32_t>& input);

int main(int argc, char* argv[]) {
    srand(time(NULL));
    size_t problem_size = std::pow(2, argc >= 2 ? atoi(argv[1]) : 3);
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
    printf("Execution time = %f ms\n", serial_time.count());
}

std::vector<int32_t> scan(const std::vector<int32_t>& input) {

    std::vector<int32_t> result(input.size());
    result[0] = input[0];

    for (size_t i = 1; i < input.size(); ++i) {
        result[i] = input[i] + result[i - 1];
    }

    return result;
}