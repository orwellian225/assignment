#include <vector>
#include <cstddef>
#include <cstdint>
#include <thread>
#include <chrono>

#include "util.h"
#include "main.h"

using namespace std::chrono_literals;

std::vector<uint32_t> sssp_mpi(size_t source, size_t vertex_count, std::vector<uint32_t> weights) {
    return std::vector<uint32_t>(vertex_count);
}