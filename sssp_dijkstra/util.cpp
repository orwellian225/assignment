#include <cstddef>
#include <cstdint>
#include <stdio.h>
#include <vector>

#include "util.h"
#include "main.h"

size_t idx_to_x(size_t idx, size_t width) { return idx % width; }
size_t idx_to_y(size_t idx, size_t width) { return idx / width; }
size_t xy_to_idx(size_t x, size_t y, size_t width) { return y * width + x; }

size_t search_visited(const std::vector<size_t>& visited, size_t vertex) {
    for (size_t i = 0; i < visited.size(); ++i) {
        if (visited[i] == vertex) {
            return i;
        }
    }

    return -1;
}

void print_distance(const std::vector<path_t>& serial, const std::vector<path_t>& omp, const std::vector<path_t>& mpi, size_t source_vertex) {
    printf("Source Vertex: | Destination Vertex | Distance Serial | Distance OMP | Distance MPI\n");
    for (size_t i = 0; i < serial.size(); ++i) {
        if (i == source_vertex) {
            continue;
        }
        printf("%ld | %ld | %d | %d | %d\n", source_vertex, i, serial[i].distance, mpi[i].distance, mpi[i].distance);
    }
}

void print_path(const std::vector<path_t>& serial, const std::vector<path_t>& omp, const std::vector<path_t>& mpi, size_t source_vertex) {
    printf("Source Vertex: | Destination Vertex | Path Serial | Path OMP | Path MPI\n");
    for (size_t i = 0; i < serial.size(); ++i) {
        if (i == source_vertex) {
            continue;
        }
        printf("%ld | %ld | %s | %s | %s\n", source_vertex, i, serial[i].path_string().c_str(), omp[i].path_string().c_str(), mpi[i].path_string().c_str());
    }
}