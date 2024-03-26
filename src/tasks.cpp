#include <vector>
#include <stdint.h>
#include <algorithm>

#include "tasks.hpp"
#include "serial.hpp"
#include "query.hpp"
#include "VecN.hpp"

void tasks::determine_queries(std::vector<Query>& queries, std::vector<VecN>& points, uint32_t num_points_per_query, Times *breakdown) {
    breakdown->distance_time_ms = 0.;
    breakdown->sort_time_ms = 0.;
    for (size_t i = 0; i < queries.size(); ++i) {
        const VecN query_point = queries[i].query;

        std::vector<QueryDistance> distances;

        auto distance_start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < points.size(); ++i) {
            QueryDistance new_qd = { 
                query_point.distance_to(points[i]),
                &points[i]
            };
            distances.push_back(new_qd);
        }
        auto distance_end = std::chrono::high_resolution_clock::now();
        breakdown->distance_time_ms += std::chrono::duration_cast<std::chrono::microseconds>(distance_end - distance_start).count() / 1000.;

        auto sort_start = std::chrono::high_resolution_clock::now();
        #pragma omp parallel
            #pragma omp single task nowait
            tasks::quicksort(distances, 0, distances.size() - 1, 1000);
        auto sort_end = std::chrono::high_resolution_clock::now();
        breakdown->sort_time_ms += std::chrono::duration_cast<std::chrono::microseconds>(sort_end - sort_start).count() / 1000.;

        std::vector<QueryDistance> selected_points;
        for (uint32_t j = 0; j < num_points_per_query; ++j) {
            selected_points.push_back(distances[j]);
        }

        queries[i].points = selected_points;
    }
}

void tasks::quicksort(std::vector<QueryDistance>& distances, int32_t low, int32_t high, int32_t limit) {
    if (low >= high || low < 0)
        return;

    if (high - low < limit) {
        serial::quicksort(distances, low, high);
        return;
    }

    QueryDistance& pivot = distances[(low + high) / 2];   
    int32_t i = low, j = high;
    while (i <= j) {
        while (distances[i].distance < pivot.distance)
            ++i;

        while(distances[j].distance > pivot.distance)
            --j;

        if (i <= j) {
            distances[i].swap(distances[j]);
            ++i;
            --j;
        }
    }

    if (low < j) {
        #pragma omp task shared(distances, low, i)
        serial::quicksort(distances, low, i - 1);
    }

    if (i < high) {
        #pragma omp task shared(distances i, high)
        serial::quicksort(distances, i + 1, high);
    }
}
