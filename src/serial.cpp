#include <tuple>
#include <algorithm>
#include <vector>

#include "serial.hpp"
#include "VecN.hpp"
#include "auxillary.hpp"

std::vector<QueryPoints> serial::nearest_neigbours(VecNArray points, VecNArray queries, size_t k) {
    std::vector<QueryPoints> result;

    for (size_t i = 0; i < queries.size; ++i) {
        QueryPoints query_result;
        query_result.query = queries[i];

        std::vector<std::pair<float, VecN*>> distances(points.size);
        for (size_t j = 0; j < points.size; ++j) {
            distances[j] = { queries[i]->norm2(*points[j]), points[j] };
        }

        serial::quicksort(distances, 0, distances.size() - 1);

        for (size_t j = 0; j < k; ++j) {
            query_result.points.push_back(distances[j].second);
        }

        result.push_back(query_result);
    }

    return result;
}

size_t serial::partition(std::vector<std::pair<float, VecN*>> arr, size_t start, size_t end) {
    std::pair<float, VecN*> *pivot = &arr[start];

    size_t count = 0;
    for (size_t i = start + 1; i <= end; ++i) {
        if (arr[i].first < pivot->first)
            ++count;
    }

    size_t pivot_i = start + count;
    std::swap(arr[pivot_i], arr[start]);

    size_t i = start, j = end;
    while (i < pivot_i && j > pivot_i) {
        while (arr[i].first <= pivot->first)
            ++i;

        while (arr[j].first > pivot->first)
            ++j;

        if (i < pivot_i && j > pivot_i)
            std::swap(arr[i++], arr[j--]);
    }

    return pivot_i;
}

void serial::quicksort(std::vector<std::pair<float, VecN*>> arr, size_t start, size_t end) {
    if (start >= end)
        return;

    size_t pivot_i = serial::partition(arr, start, end);

    serial::quicksort(arr, start, pivot_i - 1);
    serial::quicksort(arr, pivot_i + 1, end);
}