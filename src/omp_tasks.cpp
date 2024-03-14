#include <tuple>
#include <algorithm>
#include <vector>

#include <omp.h>

#include "omp_tasks.hpp"
#include "serial.hpp"
#include "VecN.hpp"
#include "auxillary.hpp"

std::vector<QueryPoints> omp_tasks::nearest_neigbours(VecNArray points, VecNArray queries, size_t k) {
    std::vector<QueryPoints> result;

    for (size_t i = 0; i < queries.size; ++i) {
        QueryPoints query_result;
        query_result.query = queries[i];

        std::vector<std::pair<float, VecN*>> distances(points.size);
        for (size_t j = 0; j < points.size; ++j) {
            distances[j] = { queries[i]->norm2(*points[j]), points[j] };
        }

#pragma omp parallel
#pragma omp single nowait
        omp_tasks::quicksort(distances, 0, distances.size() - 1, 1000);

        for (size_t j = 0; j < k; ++j) {
            query_result.points.push_back(distances[j].second);
        }

        result.push_back(query_result);
    }

    return result;
}

size_t omp_tasks::partition(std::vector<std::pair<float, VecN*>> &arr, int64_t start, int64_t end) {
    std::pair<float, VecN*> pivot = arr[end];

    size_t temp_pivot = start - 1;

    for (size_t i = start; i < end; ++i) {
        if (arr[i].first <= pivot.first) {
            ++temp_pivot;

            std::pair<float, VecN*> temp = arr[temp_pivot];
            arr[temp_pivot] = arr[i];
            arr[i] = temp;
        }
    }

    ++temp_pivot;

    std::pair<float, VecN*> temp = arr[temp_pivot];
    arr[temp_pivot] = arr[end];
    arr[end] = temp;

    return temp_pivot;
}

void omp_tasks::quicksort(std::vector<std::pair<float, VecN*>> &arr, int64_t start, int64_t end, int64_t serial_cutoff) {
    if (end <= start)
        return;

    if (end - start < serial_cutoff) {
        serial::quicksort(arr, start, end);
        return;
    }

    size_t pivot_i = omp_tasks::partition(arr, start, end);

#pragma omp task
    omp_tasks::quicksort(arr, start, pivot_i - 1, serial_cutoff);

#pragma omp task
    omp_tasks::quicksort(arr, pivot_i + 1, end, serial_cutoff);
}
