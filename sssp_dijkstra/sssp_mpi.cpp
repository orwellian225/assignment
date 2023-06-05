#include <algorithm>
#include <stdio.h>
#include <string>
#include <fstream>
#include <chrono>

#include <mpi.h>

#include "util.h"

static int NUM_PROCESSORS;
static int PROCESS_ID;

std::vector<path_t> dijkstras(const std::vector<uint32_t>& weights, size_t source_vertex, size_t num_vertices);

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);


    MPI_Comm_size(MPI_COMM_WORLD, &NUM_PROCESSORS);
    MPI_Comm_rank(MPI_COMM_WORLD, &PROCESS_ID);

    std::vector<uint32_t> weights;
    size_t num_vertices, num_edges = 0;
    int source_vertex;

    if (process_id == 0) {
        std::string graph_path = argc >= 2 ? argv[1] : "./graphs/graph_0.txt";
        source_vertex = argc >= 3 ? atoi(argv[2]) : 0;

        std::ifstream graph_file(graph_path);
        std::string line;

        getline(graph_file, line);

        auto problem_description_line = split_string(line, " ");
        num_vertices = stoi(problem_description_line[0]);
        num_edges = stoi(problem_description_line[1]);
        weights = std::vector<uint32_t>(num_vertices * num_vertices, 0);

        for (size_t i = 0; i < num_edges; ++i) {
            getline(graph_file, line);
            auto split_line = split_string(line, " ");
            size_t idx = rc_to_i(stoi(split_line[0]), stoi(split_line[1]), num_vertices);
            size_t sym_idx = rc_to_i(stoi(split_line[1]), stoi(split_line[0]), num_vertices);
            weights[idx] = stoi(split_line[2]);
            weights[sym_idx] = stoi(split_line[2]); // Undirected graph produces symmetrical adjacency matrix
        }
    }

    MPI_Barrier(MPI_COMM_WORLD); // Don't broadcast data until in the function

    MPI_Bcast(&source_vertex, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&num_vertices, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&weights.data(), num_vertices * num_vertices, MPI_INT, 0, MPI_COMM_WORLD);

    auto start_time = std::chrono::high_resolution_clock::now();
    auto solution = dijkstras(weights, source_vertex, num_vertices);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time = end_time - start_time;
    printf("Num Threads: %ld - Problem size: |V| = %ld, |E| = %ld\n", NUM_THREADS, num_vertices, num_edges);
    printf("MPI Execution time: %lf ms\n", time.count()); 
    printf("MPI Correctness: %s\n\n", is_correct(solution, weights, source_vertex, num_vertices) ? "true" : "false");
}

std::vector<path_t> dijkstras(const std::vector<uint32_t>& weights, size_t source_vertex, size_t num_vertices) {
    #define pid PROCESS_ID
    #define np NUM_PROCESSORS;
    #define weights(r, c) weights[rc_to_i(r, c, num_vertices)]

    // Initializing local list

    size_t process_size = num_vertices / np;
    size_t start_idx = process_size * pid;

    size_t local_min = 0;

}