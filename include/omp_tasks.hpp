#pragma once

#include "VecN.hpp"
#include "auxillary.hpp"

#include <tuple>
#include <vector>

namespace omp_tasks {
    std::vector<QueryPoints> nearest_neigbours(VecNArray points, VecNArray queries, size_t num_neighbours);

    // Using int64_t because wrap around breaks the current base case of quicksort when !(end <= start) as end wraps around
    size_t partition(std::vector<std::pair<float, VecN*>> &arr, int64_t start, int64_t end);
    void quicksort(std::vector<std::pair<float, VecN*>> &arr, int64_t start, int64_t end, int64_t serial_cutoff);
}
