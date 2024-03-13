#pragma once

#include "VecN.hpp"
#include "auxillary.hpp"

#include <tuple>
#include <vector>

namespace serial {
    std::vector<QueryPoints> nearest_neigbours(VecNArray points, VecNArray queries, size_t num_neighbours);

    size_t partition(std::vector<std::pair<float, VecN*>> arr, size_t start, size_t end);
    void quicksort(std::vector<std::pair<float, VecN*>> arr, size_t start, size_t end);
}
