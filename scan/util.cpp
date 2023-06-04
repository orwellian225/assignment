#include <vector>
#include <stdint.h>
#include <stdio.h>

#include "util.h"
#include "problem.h"

bool is_correct(const std::vector<int32_t>& input, const std::vector<int32_t>& solution) {

    std::vector<int32_t> correct_solution = scan_serial(input);

    for (size_t i = 0; i < correct_solution.size(); ++i) {
        if (solution[i] != correct_solution[i]) { return false; }
    }

    return true;
}

void print_vec(const std::vector<int32_t>& input) {
    for (auto val: input) {
        printf("%d, ", val);
    }
    printf("\n");
}