#include <vector>
#include <string>
#include <stdint.h>
#include <stddef.h>
#include <tuple>

#include "util.h"

bool path_t::operator== (const path_t& rhs) {
    if (distance != rhs.distance) {
        return false;
    }

    if (path.size() != rhs.path.size()) {
        return false;
    }

    for (size_t i = 0; i < path.size(); ++i) {
        if (path[i] != rhs.path[i]) {
            return false;
        }
    }

    return true;
}

bool path_t::operator!= (const path_t& rhs) {
    return !(*this == rhs);
}

std::vector<std::string> split_string(std::string input, std::string delimiter) {
    std::vector<std::string> result;

    size_t pos = 0;
    std::string token;
    while ((pos = input.find(delimiter)) != std::string::npos) {
        token = input.substr(0, pos);
        result.push_back(token);
        input.erase(0, pos + delimiter.length());
    }
    result.push_back(input); // Add the last element

    return result;
}

size_t rc_to_i(size_t row, size_t col, size_t width) { return static_cast<size_t>(row * width + col); }
std::pair<size_t, size_t> i_to_rc(size_t i, size_t width) { return std::make_pair(static_cast<size_t>(i / width), static_cast<size_t>(i % width)); }

bool and_all(const std::vector<bool>& input) {
    for (auto val: input) {
        if (!val) { return false; }
    }
    return true;
}

std::string vec_to_str(const std::vector<size_t>& input) {
    std::string result = "";

    for (size_t i = 0; i < input.size(); ++i) {
        result += std::to_string(input[i]) + " ";
    }

    if (result.size() >= 1) 
        result.erase(result.size() - 1);

    return result;
}

std::vector<path_t> dijkstras_serial(const std::vector<uint32_t>& weights, size_t source_vertex, size_t num_vertices) {
    #define weights(r, c) weights[rc_to_i(r, c, num_vertices)]

    std::vector<bool> visited(num_vertices, false);
    std::vector<path_t> paths(num_vertices);

    visited[source_vertex] = true;

    for (size_t i = 0; i < num_vertices; ++i) {
        if (visited[i]) { continue; }

        paths[i] = path_t {
            weights(source_vertex, i) == 0 ? UINT32_MAX : weights(source_vertex, i), // Add the distance to the vertex
            weights(source_vertex, i) == 0 ? std::vector<size_t>() : std::vector<size_t>(1, i) // Add the vertex to the path
        };
    }

    while (!and_all(visited)) {
        size_t u = num_vertices + 1;
        // Find vertex u
        for (size_t i = 0; i < num_vertices; ++i) {
            if (visited[i]) { continue; }
            if (u >= num_vertices + 1) { u = i; } // Some error handling to correctly initialize u
            u = paths[i].distance < paths[u].distance ? i : u;
        }

        visited[u] = true;
        for (size_t i = 0; i < num_vertices; ++i) {
            if (visited[i]) { continue; }
            uint32_t weight = weights(u, i);
            if ( weights(u, i) != 0 && paths[i].distance > paths[u].distance + weights(u, i)) {
                paths[i].distance = paths[u].distance + weights(u, i);
                paths[i].path = paths[u].path;
                paths[i].path.push_back(i);
            } 
        }
    }

    return paths;
}

bool is_correct(const std::vector<path_t>& solution, const std::vector<uint32_t>& weights, size_t source_vertex, size_t num_vertices) {

    std::vector<path_t> correct_solution = dijkstras_serial(weights, source_vertex, num_vertices);

    if (correct_solution.size() != solution.size()) {
        return false;
    }

    for (size_t i = 0; i < correct_solution.size(); ++i) {
        if (correct_solution[i] != solution[i]) {
            return false;
        }
    }

    return true;
}