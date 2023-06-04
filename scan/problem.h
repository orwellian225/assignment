#pragma once

#include <vector>
#include <stdint.h>
#include <stddef.h>

std::vector<int32_t> scan_serial(const std::vector<int32_t>& input);
std::vector<int32_t> scan_omp(const std::vector<int32_t>& input);
std::vector<int32_t> scan_mpi(const std::vector<int32_t>& input);