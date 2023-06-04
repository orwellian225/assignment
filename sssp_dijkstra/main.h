#pragma once

#include <vector>
#include <string>
#include <cstddef>

struct path_t {
    size_t source_vertex;
    uint32_t distance;
    std::vector<size_t> path;

    std::string path_string() const {
        std::string result = "";
        for (size_t vertex: path) {
            result += vertex + ", ";
        }
        return result;
    }
};

std::vector<path_t> sssp_serial(size_t source, size_t vertex_count, std::vector<uint32_t> weights);
std::vector<path_t> sssp_omp(size_t source, size_t vertex_count, std::vector<uint32_t> weights);
std::vector<path_t> sssp_mpi(size_t source, size_t vertex_count, std::vector<uint32_t> weights);