#include <algorithm>
#include <stdio.h>
#include <string>
#include <fstream>
#include <chrono>

#include <omp.h>

#include "util.h"

static size_t NUM_THREADS;

/// path_t struct can be found in util.h
/// Definition
/// struct path_t {
///     uint32_t distance;
///     std::vector<size_t> path;
/// }
std::vector<path_t> dijkstras(const std::vector<uint32_t>& weights, size_t source_vertex, size_t num_vertices);

int main(int argc, char* argv[]) {
    std::string graph_path = argc >= 2 ? argv[1] : "./graphs/graph_0.txt";
    size_t source_vertex = argc >= 3 ? atoi(argv[2]) : 0;
    NUM_THREADS = argc >= 4 ? atoi(argv[3]) : omp_get_max_threads();

    std::vector<uint32_t> weights;
    size_t num_vertices, num_edges = 0;

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

    // If there are more threads than processors, just use 2 processors, so its split down the middle
    NUM_THREADS = NUM_THREADS < num_vertices ? NUM_THREADS : 2; 

    omp_set_num_threads(NUM_THREADS);

    auto start_time = std::chrono::high_resolution_clock::now();
    auto solution = dijkstras(weights, source_vertex, num_vertices);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time = end_time - start_time;
    printf("Problem: |V| = %ld, |E| = %ld\n", num_vertices, num_edges);
    printf("OpenMP Execution time: %lf ms\n", time.count()); 
    printf("OpenMP Correctness: %s\n", is_correct(solution, weights, source_vertex, num_vertices) ? "true" : "false");
}

std::vector<path_t> dijkstras(const std::vector<uint32_t>& weights, size_t source_vertex, size_t num_vertices) {
    #define weights(r, c) weights[rc_to_i(r, c, num_vertices)]

    std::vector<bool> visited(num_vertices, false);
    std::vector<path_t> paths(num_vertices);

    visited[source_vertex] = true;

    #pragma parallel omp for
    for (size_t i = 0; i < num_vertices; ++i) {
        if (visited[i]) { continue; }

        paths[i] = path_t {
            weights(source_vertex, i) == 0 ? UINT32_MAX : weights(source_vertex, i), // Add the distance to the vertex
            weights(source_vertex, i) == 0 ? std::vector<size_t>() : std::vector<size_t>(1, i) // Add the vertex to the path
        };
    }

    while (!and_all(visited)) {
        size_t thread_size = num_vertices / NUM_THREADS;
        std::vector<size_t> thread_mins(NUM_THREADS, num_vertices + 1);

        #pragma omp parallel
        {
            size_t thread_id = omp_get_thread_num();
            size_t start_idx = thread_id * thread_size;
            // printf("PID %ld, executing from %ld till %ld\n", thread_id, start_idx, start_idx + thread_size - 1);

            for (size_t i = 0; i < thread_size; ++i) {
                if (visited[start_idx + i]) { continue; }
                if (thread_mins[thread_id] >= num_vertices + 1) { thread_mins[thread_id] = start_idx + i; } // Some error handling to correctly initialize u
                thread_mins[thread_id] = paths[start_idx + i].distance < paths[thread_mins[thread_id]].distance ? start_idx + i : thread_mins[thread_id];
            }
        }

        size_t u = num_vertices + 1;
        for (size_t i = 0; i < thread_mins.size(); ++i) {
            if (visited[thread_mins[i]]) { continue; }
            if (thread_mins[i] == num_vertices + 1) { continue; } // Check if a local min was actually found, if not just move on
            if (u >= num_vertices + 1) { u = thread_mins[i]; } // Some error handling to correctly initialize u
            u = paths[thread_mins[i]].distance < paths[u].distance ? thread_mins[i] : u;
        }

        visited[u] = true;

        #pragma omp parallel for
        for (size_t i = 0; i < num_vertices; ++i) {
            if (visited[i]) { continue; }
            uint32_t weight = weights(u, i);
            if ( weights(u, i) != 0 && paths[i].distance > paths[u].distance + weights(u, i)) {
                paths[i].distance = paths[u].distance + weights(u, i);
                paths[i].path = paths[u].path;
                paths[i].path.push_back(i);
            } 
        }
    }

    return paths;
}