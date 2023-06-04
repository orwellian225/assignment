#include <vector>
#include <cstddef>
#include <cstdint>
#include <thread>
#include <chrono>

#include "util.h"
#include "main.h"

std::vector<path_t> sssp_serial(size_t source, size_t vertex_count, std::vector<uint32_t> weights) {
    std::vector<size_t> visited = { source }; 
    std::vector<path_t> paths(vertex_count);

    for(size_t i = 0; i < vertex_count; ++i) {
        // Search through the frontier, and if the the current vertex is found in frontier, then move to the next vertex
        if (search_visited(visited, i) != -1) { continue; }

        // If the direct path doesn't exist (== 0) to the vertex, then set its loss to be infinity (or thereabouts)
        uint32_t weight = weights[xy_to_idx(source, i, vertex_count)];
        paths[i].distance = weight == 0 ? UINT32_MAX : weight;
        paths[i].source_vertex = source;
    }

    while (visited.size() != vertex_count) {

        uint32_t min_val = UINT32_MAX;
        size_t min_vertex = 0; // Can start at 0, because we still do the comparison at 0, so it'll get updated to its correct loss value
        for (size_t i = 0; i < vertex_count; ++i) {
            if (search_visited(visited, i) != -1) { continue; }

            min_val = min_val < paths[i].distance ? min_val : paths[i].distance;
            min_vertex = min_val < paths[i].distance ? min_vertex : i;
        }

        visited.push_back(min_vertex);

        for (size_t i = 0; i < vertex_count; ++i) {
            if (search_visited(visited, i) != -1) { continue; }

            size_t weight_idx = xy_to_idx(min_vertex, i, vertex_count);
            paths[i].distance = paths[i].distance < paths[min_vertex].distance + weights[weight_idx] ? paths[i].distance : paths[min_vertex].distance + weights[weight_idx];

            if (paths[i].distance < paths[min_vertex].distance + weights[weight_idx]) {
                paths[i] = paths[min_vertex];
                paths[i].distance += weights[weight_idx];
                paths[i].path.push_back(i);
            }
        }

    }

    return paths;
}