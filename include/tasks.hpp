#pragma once

#include <vector>
#include <stdint.h>

#include "query.hpp"
#include "VecN.hpp"

namespace tasks {
    void determine_queries(std::vector<Query>& queries, std::vector<VecN>& points, uint32_t num_points_per_query);
    void quicksort(std::vector<QueryDistance>& distances, int32_t low, int32_t high, int32_t limit);
}
