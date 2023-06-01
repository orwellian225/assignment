#include <vector>
#include <cstddef>
#include <cstdint>
#include <thread>
#include <chrono>

#include "util.h"
#include "main.h"

// Return the position of the vertex in the visited
// Returns -1 if the vertex is not found
size_t search_visited(const std::vector<size_t>& visited, size_t vertex) {
    for (size_t i = 0; i < visited.size(); ++i) {
        if (visited[i] == vertex) {
            return i;
        }
    }

    return -1;
}

std::vector<uint32_t> sssp_serial(size_t source, size_t vertex_count, std::vector<uint32_t> weights) {
    std::vector<size_t> visited = { source }; 
    std::vector<uint32_t> loss(vertex_count);

    for(size_t i = 0; i < vertex_count; ++i) {
        // Search through the frontier, and if the the current vertex is found in frontier, then move to the next vertex
        if (search_visited(visited, i) != -1) { continue; }

        // If the direct path doesn't exist (== 0) to the vertex, then set its loss to be infinity (or thereabouts)
        uint32_t weight = weights[xy_to_idx(source, i, vertex_count)];
        loss[i] = weight == 0 ? UINT32_MAX : weight;
    }

    while (visited.size() != vertex_count) {

        uint32_t min_val = UINT32_MAX;
        size_t min_vertex = 0; // Can start at 0, because we still do the comparison at 0, so it'll get updated to its correct loss value
        for (size_t i = 0; i < vertex_count; ++i) {
            if (search_visited(visited, i) != -1) { continue; }

            min_val = min_val < loss[i] ? min_val : loss[i];
            min_vertex = min_val < loss[i] ? min_vertex : i;
        }

        visited.push_back(min_vertex);

        for (size_t i = 0; i < vertex_count; ++i) {
            if (search_visited(visited, i) != -1) { continue; }

            size_t weight_idx = xy_to_idx(min_vertex, i, vertex_count);
            loss[i] = loss[i] < loss[min_vertex] + weights[weight_idx] ? loss[i] : loss[min_vertex] + weights[weight_idx];
        }

    }

    return loss;
}