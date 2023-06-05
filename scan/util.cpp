#include <vector>
#include <stdint.h>
#include <stdio.h>

#include "util.h"

bool is_correct(const std::vector<int32_t>& input, const std::vector<int32_t>& solution) {

    if (input.size() != solution.size()) {
        return false;
    }

    int32_t sum = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        sum += input[i];

        if (solution[i] != sum) {
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