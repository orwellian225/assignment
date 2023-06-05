#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <chrono>
#include <stddef.h>

#include <omp.h>

#include "util.h"

#define pow2(x) std::pow(2, x)

static size_t NUM_THREADS;

void up_sweep(std::vector<int32_t>& input);
void down_sweep(std::vector<int32_t>& input);
std::vector<int32_t> scan(const std::vector<int32_t>& input);

int main(int argc, char* argv[]) {
    srand(time(NULL));
    size_t problem_size = std::pow(2, argc >= 2 ? atoi(argv[1]) : 3);
    NUM_THREADS = argc >= 3 ? atoi(argv[2]) : omp_get_max_threads();
    std::vector<int32_t> problem(problem_size);

    omp_set_num_threads(omp_get_max_threads());
    #pragma omp for
    for (size_t j = 0; j < problem_size; ++j) {
        problem[j] = rand() % (10 * problem_size);

    }

    printf("Num Threads: %ld - Problem size: %ld\n", NUM_THREADS, problem_size);
    auto start = std::chrono::high_resolution_clock::now();
    auto solution = scan(problem);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time= end - start;
    printf("Open MP Execution time = %f ms\n", time.count());
    printf("OpenMP Correctness: %s\n\n", is_correct(problem, solution) ? "true" : "false");
}

void up_sweep(std::vector<int32_t>& input) {
    size_t max_depth = static_cast<size_t>(std::log2(input.size()));

    for (size_t d = 0; d < max_depth; ++d) {

        size_t p2_0 = static_cast<size_t>(pow2(d));
        size_t p2_1 = static_cast<size_t>(pow2(d + 1));

        #pragma omp for
        for (size_t i = 0; i < input.size(); i += p2_1) {
            input[i + p2_1 - 1] += input[i + p2_0 - 1];
        }
    }
}

void down_sweep(std::vector<int32_t>& input) {
    size_t max_depth = static_cast<size_t>(std::log2(input.size()));
    input[input.size() - 1] = 0;

    for (size_t d = max_depth; d-- > 0; ) {
        size_t p2_0 = static_cast<size_t>(pow2(d));
        size_t p2_1 = static_cast<size_t>(pow2(d + 1));

        #pragma omp for
        for (size_t i = 0; i < input.size(); i += p2_1) {
            int32_t temp = input[i + p2_0 - 1];
            input[i + p2_0 - 1] = input[i + p2_1 - 1];
            input[i + p2_1 - 1] += temp;
        }
    }

}

std::vector<int32_t> scan(const std::vector<int32_t>& input) {

    // Either set the number of threads to the min of number of elements or number of available threads
    size_t num_threads = input.size() < NUM_THREADS ? input.size() : NUM_THREADS;
    omp_set_num_threads(num_threads);

    std::vector<int32_t> result(input);
    std::vector<int32_t> sum(num_threads);
    size_t thread_size = result.size() / num_threads; // Length of sublist in thread

    // size_t is u64 or u32 so cheap to clone
    // Execute a serial scan on each processors sublist
    #pragma omp parallel shared(result, sum, input)
    { 
        size_t thread_id = omp_get_thread_num();
        size_t start_idx = thread_id * thread_size; // Starting index of the sublist

        for (size_t j = 1; j < thread_size; ++j) {
            result[start_idx + j] = result[start_idx + j] + result[start_idx + j - 1];
        }

        sum[thread_id] = result[start_idx + thread_size - 1];
    }

    up_sweep(sum);
    down_sweep(sum);

    // Apply the sum to all the thread sublists    
    #pragma omp parallel shared(sum, result)
    {
        size_t thread_id = omp_get_thread_num();
        size_t start_idx = thread_id * thread_size; // Starting index of the sublist

        for (size_t i = 0; i < thread_size; ++i) {
            result[start_idx + i] += sum[thread_id];
        }
    }

    return result;
}