#pragma once

#include "VecN.hpp"
#include "auxillary.hpp"

namespace serial {
    size_t partition(DataClass **points, size_t start, size_t end);
    void qsort(DataClass **points, size_t start, size_t end);
    DataClass **nearest_neighbours(VecN **data, size_t num_neighbourhoods);    
}
