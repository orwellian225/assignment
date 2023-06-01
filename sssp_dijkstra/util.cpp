#include <cstddef>

#include "util.h"

size_t idx_to_x(size_t idx, size_t width) { return idx % width; }
size_t idx_to_y(size_t idx, size_t width) { return idx / width; }
size_t xy_to_idx(size_t x, size_t y, size_t width) { return y * width + x; }
