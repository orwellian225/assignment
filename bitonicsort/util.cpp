#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <stdio.h>

bool is_correct(std::vector<int32_t>& sorted_array) {
    for (size_t i = 0; i < sorted_array.size() - 1; ++i) {
        if (sorted_array[i] > sorted_array[i + 1]) {
            return false;
        }
    }

    return true;
}

void print_vec(const std::vector<int32_t>& input) {
    for (auto val: input) {
        printf("%d, ", val);
    }
    printf("\n");
}