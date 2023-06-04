#include "problem.h"

std::vector<int32_t> scan_omp(const std::vector<int32_t>& input) {

    std::vector<int32_t> result(input.size());
    result[0] = input[0];

    for (size_t i = 1; i < input.size(); ++i) {
        result[i] = input[i] + result[i - 1];
    }

    return result;
}