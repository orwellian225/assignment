#pragma once

#include <cstddef>

#include "main.h"

size_t idx_to_x(size_t idx, size_t width);
size_t idx_to_y(size_t idx, size_t width);
size_t xy_to_idx(size_t x, size_t y, size_t width);

// Return the position of the vertex in the visited
// Returns -1 if the vertex is not found
size_t search_visited(const std::vector<size_t>& visited, size_t vertex);


void print_distance(const std::vector<path_t>& serial, const std::vector<path_t>& omp, const std::vector<path_t>& mpi, size_t source_vertex);
void print_path(const std::vector<path_t>& serial, const std::vector<path_t>& omp, const std::vector<path_t>& mpi, size_t source_vertex);