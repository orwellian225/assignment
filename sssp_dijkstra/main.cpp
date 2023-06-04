#include <stdio.h>
#include <filesystem>
#include <chrono>
#include <vector>
#include <fstream>

#include "main.h"
#include "util.h"

#define SOURCE_VERTEX 0

std::vector<std::string> split_string(std::string input, std::string delimiter) {
    std::vector<std::string> result;

    size_t pos = 0;
    std::string token;
    while ((pos = input.find(delimiter)) != std::string::npos) {
        token = input.substr(0, pos);
        result.push_back(token);
        input.erase(0, pos + delimiter.length());
    }
    result.push_back(input); // Add the last element

    return result;
}

int main() {
    printf("Dijkstra's Single Source Shortest Path Algorithm\n");
    printf("\tAll calculations done from source Vertex %d\n\n", SOURCE_VERTEX);

    std::vector<std::string> graphs = {
        "./data/graph_0.txt", "./data/graph_1.txt", "./data/graph_2.txt",
        "./data/graph_3.txt", "./data/graph_4.txt", "./data/graph_5.txt",
        "./data/graph_6.txt", "./data/graph_7.txt", "./data/graph_8.txt",
        "./data/graph_9.txt", "./data/graph_10.txt"
    };

    for (auto graph_filepath: graphs) {
        // Handle file input here
        std::vector<uint32_t> weight_matrix_1d;
        size_t vertex_count = 0;
        size_t edge_count = 0;

        std::ifstream file(graph_filepath);
        std::string line;

        getline(file, line);
        std::vector<std::string> problem_line = split_string(line, " ");
        vertex_count = stoi(problem_line[0]);
        edge_count = stoi(problem_line[1]);
        weight_matrix_1d = std::vector<uint32_t>(vertex_count * edge_count, 0);

        for (size_t i = 0; i < edge_count; ++i) {
            getline(file, line);            
            std::vector<std::string> edge_line = split_string(line, " ");
            size_t x = stoi(edge_line[0]);
            size_t y = stoi(edge_line[1]);
            uint32_t weight = stoi(edge_line[2]);
            size_t idx = xy_to_idx(x, y, vertex_count);
            size_t idx_sym = xy_to_idx(y, x, vertex_count);

            weight_matrix_1d[idx] = weight;
            weight_matrix_1d[idx_sym] = weight; // graph is undirected therefore symmetrical
        }

        printf("Problem: %s\n", graph_filepath.c_str());
        printf("Problem Size: |V| = %ld, |E| = %ld\n", vertex_count, edge_count);

        // Pass to serial here

        auto serial_start = std::chrono::high_resolution_clock::now();
        std::vector<path_t> serial_solution = sssp_serial(SOURCE_VERTEX, vertex_count, weight_matrix_1d);
        auto serial_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> serial_time = serial_end - serial_start;
        printf("Serial Execution time = %f ms\n", serial_time.count());

        // Pass to OMP here
        auto omp_start = std::chrono::high_resolution_clock::now();
        std::vector<path_t> omp_solution = sssp_omp(SOURCE_VERTEX, vertex_count, weight_matrix_1d);
        auto omp_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> omp_time = omp_end - omp_start;
        double omp_speedup = serial_time.count() / omp_time.count();
        printf("OMP Execution time = %f ms, OMP Speedup = %lf\n", omp_time.count(), omp_speedup);

        // Pass to MPI here
        auto mpi_start = std::chrono::high_resolution_clock::now();
        std::vector<path_t> mpi_solution = sssp_mpi(SOURCE_VERTEX, vertex_count, weight_matrix_1d);
        auto mpi_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> mpi_time = mpi_end - mpi_start;
        double mpi_speedup = serial_time.count() / mpi_time.count();
        printf("MPI Execution time = %f ms, MPI Speedup = %lf\n", mpi_time.count(), mpi_speedup);

        if (vertex_count <= 10) {
            print_distance(serial_solution, omp_solution, mpi_solution, SOURCE_VERTEX);
            printf("\n");
            print_path(serial_solution, omp_solution, mpi_solution, SOURCE_VERTEX);
        }
        printf("\n");
    }
}