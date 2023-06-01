#pragma once

#include <cstddef>

size_t idx_to_x(size_t idx, size_t width);
size_t idx_to_y(size_t idx, size_t width);
size_t xy_to_idx(size_t x, size_t y, size_t width);