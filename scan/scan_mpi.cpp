#include <math.h>
#include <stdio.h>
#include <chrono>
#include <stdlib.h>
#include <mpi.h>

#include "util.h"

#define pow2i(x) static_cast<size_t>(std::floor(std::pow(2, x)))

std::vector<int32_t> scan_mpi(const std::vector<int32_t>& input, int num_processors, int process_id);

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int num_processors, process_id;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processors);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    size_t problem_size = std::pow(2, argc >= 2 ? atoi(argv[1]) : 3);
    std::vector<int32_t> problem(problem_size);

    if (process_id == 0) {
        srand(time(NULL));
        for (size_t j = 0; j < problem_size; ++j) {
            problem[j] = rand() % (10 * problem_size);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);

    auto mpi_start = std::chrono::high_resolution_clock::now();
    auto result = scan_mpi(problem, num_processors, process_id);
    auto mpi_end = std::chrono::high_resolution_clock::now();

    if (process_id == 0) {
        std::chrono::duration<double, std::milli> mpi_time = mpi_end - mpi_start;
        printf("Num Processors: %d - Problem size: %ld\n", num_processors, problem_size);
        printf("MPI Execution time = %f ms\n", mpi_time.count());
        printf("MPI Correctness %s\n\n", is_correct(problem, result) ? "true" : "false");
    }

    MPI_Finalize();
}

std::vector<int32_t> scan_mpi(const std::vector<int32_t>& input, int num_processors, int process_id) {
    std::vector<int32_t> result(input);
    std::vector<int32_t> sum(num_processors);

    MPI_Bcast(result.data(), result.size(), MPI_INT, 0, MPI_COMM_WORLD);
    int32_t process_size = result.size() / num_processors;

    // Processor Sublist
    int32_t start_idx = process_id * process_size;
    for (int32_t j = 1; j < process_size; ++j) {
        result[start_idx + j] += result[start_idx + j - 1];
    }
    sum[process_id] = result[start_idx + process_size - 1];

    /// Processor types for each step of the tree
    /// Sender --> Left Vertex of Bin Tree -> Sends its sum value to the relevant receiver
    /// Receiver --> Right Vertex of Bin Tree --> Adds the sent sum value to its own sum value

    // UP SWEEP
    int32_t max_depth = static_cast<int32_t>(std::log2(sum.size()));
    for (int32_t d = 0; d < max_depth; ++d) {
        bool is_left_processor = (process_id + 1) % pow2i(d + 1) == pow2i(d);
        bool is_right_processor = (process_id + 1) % pow2i(d + 1) == 0;

        // Receiver Rule
        if (is_right_processor) {
            int32_t left_processor = process_id - pow2i(d);
            int32_t recv_value;

            MPI_Recv(&recv_value, 1, MPI_INT, left_processor, d, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum[process_id] += recv_value;
        }

        // Sender Rule
        if (is_left_processor) {
            // Sends to 
            int32_t right_processor = process_id + pow2i(d);
            MPI_Send(&sum[process_id], 1, MPI_INT, right_processor, d, MPI_COMM_WORLD);
        }
    }

    // DOWN SWEEP
    if (process_id == sum.size() - 1) {
        sum[process_id] = 0;
    }
    for (int32_t d = max_depth; d-- > 0;) {
        bool is_left_processor = (process_id + 1) % pow2i(d + 1) == pow2i(d);
        bool is_right_processor = (process_id + 1) % pow2i(d + 1) == 0;

        // Left Vertex Rule
        if (is_left_processor) {
            int32_t right_processor = process_id + pow2i(d);
            int32_t right_val;

            MPI_Recv(&right_val, 1, MPI_INT, right_processor, d, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&sum[process_id], 1, MPI_INT, right_processor, d, MPI_COMM_WORLD);
            sum[process_id] = right_val;
        }

        // Right Vertex Rule
        if (is_right_processor) {
            int32_t left_processor = process_id - pow2i(d);
            int32_t left_value;

            MPI_Send(&sum[process_id], 1, MPI_INT, left_processor, d, MPI_COMM_WORLD);
            MPI_Recv(&left_value, 1, MPI_INT, left_processor, d, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum[process_id] += left_value;
        }
    }

    for (int32_t i = 0; i < process_size; ++i) {
        result[start_idx + i] += sum[process_id];
    }

    MPI_Gather(result.data() + start_idx, process_size, MPI_INT, result.data(), process_size, MPI_INT, 0, MPI_COMM_WORLD);
    return result;
}