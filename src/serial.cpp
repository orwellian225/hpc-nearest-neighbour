#include "serial.hpp"
#include "VecN.hpp"
#include "auxillary.hpp"

#include <algorithm>

size_t serial::partition(DataClass **points, size_t start, size_t end) {
    DataClass *pivot = points[start];

    size_t count = 0;
    for (size_t i = start + 1; i <= end; ++i) {
        if (points[start]->distance_to_class < pivot->distance_to_class)
            ++count;
    }

    size_t pivot_i = start + count;
    std::swap(points[pivot_i], points[start]);

    size_t i = start, j = end;
    while (i < pivot_i && j > pivot_i) {

        while (points[i]->distance_to_class <= pivot->distance_to_class)
            ++i;

        while (points[j]->distance_to_class > pivot->distance_to_class)
            --j;

        if (i < pivot_i && j > pivot_i)
            std::swap(points[i++], points[j--]);

    }
    
    return pivot_i;
}

void serial::qsort(DataClass **points, size_t start, size_t end) {
    if (start >= end)
        return;

    size_t pivot = serial::partition(points, start, end);
    qsort(points, start, pivot - 1);
    qsort(points, pivot + 1, end);
}

DataClass **serial::nearest_neighbours(VecN **data, size_t num_neighbourhoods) {

}
