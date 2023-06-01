#include <vector>
#include <cstddef>
#include <cstdint>
#include <thread>
#include <chrono>

#include <omp.h>

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

std::vector<uint32_t> sssp_omp(size_t source, size_t vertex_count, std::vector<uint32_t> weights) {
    std::vector<size_t> visited = { source };
    std::vector<uin32_t> loss(vertex_count);

    for(size_t i = 0; i < vertex_count; ++i) {
        // Search through the frontier, and if the the current vertex is found in frontier, then move to the next vertex
        if (search_visited(visited, i) != -1) { continue; }

        // If the direct path doesn't exist (== 0) to the vertex, then set its loss to be infinity (or thereabouts)
        uint32_t weight = weights[xy_to_idx(source, i, vertex_count)];
        loss[i] = weight == 0 ? UINT32_MAX : weight;
    }

    const size_t num_threads = omp_get_num_threads();
    const size_t partition_size = vertex_count / num_threads;
    while (visited.size() != vertex_count) {
        // Cloning partition_size is very cheap do to it being a primitive
        // Cloning weight matrix is fairly expensive, kinda want to avoid it

        std::vector<uin32_t> min_vals(num_threads);
        std::vector<size_t> min_vertices(num_threads);

        #pragma omp parallel firstprivate(partition_size) shared(min_vals, min_vertices, loss, weights)
        { // The multi-threaded part of the parallization
            size_t thread_id = omp_get_thread_num();
            size_t loss_start = thread_id * partition_size;
            size_t loss_end = loss_start + partition_size;

            for (size_t i = loss_start; i < loss_end; ++i) {
                if (search_visited(visited, i) != -1) { continue; } 

                min_vals[thread_id] = min_vals[thread_id] < loss[i] ? min_vals[thread_id] : loss[i];
                min_vertices[thread_id] = min_vertices[thread_id] < loss[i] ? min_vertices[thread_id] : i;
            }

            #pragma omp critical
            // Compute
        }
    }

}