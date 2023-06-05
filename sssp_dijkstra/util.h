#include <vector>
#include <string>
#include <stdint.h>
#include <stddef.h>
#include <tuple>

struct path_t {
    uint32_t distance;
    std::vector<size_t> path;

    bool operator == (const path_t& rhs);
    bool operator != (const path_t& rhs);
};

std::vector<std::string> split_string(std::string input, std::string delimiter);
size_t rc_to_i(size_t row, size_t col, size_t width);
std::pair<size_t, size_t> i_to_rc(size_t i, size_t width);

bool and_all(const std::vector<bool>& input);

std::string vec_to_str(const std::vector<size_t>& input);

std::vector<path_t> dijkstras_serial(const std::vector<uint32_t>& weights, size_t source_vertex, size_t num_vertices);
bool is_correct(const std::vector<path_t>& solution, const std::vector<uint32_t>& weights, size_t source_vertex, size_t num_vertices);
