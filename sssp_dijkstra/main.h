#pragma once

#include <vector>
#include <cstddef>

std::vector<uint32_t> sssp_serial(size_t source, size_t vertex_count, std::vector<uint32_t> weights);
std::vector<uint32_t> sssp_omp(size_t source, size_t vertex_count, std::vector<uint32_t> weights);
std::vector<uint32_t> sssp_mpi(size_t source, size_t vertex_count, std::vector<uint32_t> weights);