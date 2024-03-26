#include <chrono>
#include <vector>
#include <stdint.h>
#include <algorithm>

#include "serial.hpp"
#include "Times.hpp"
#include "query.hpp"
#include "VecN.hpp"

void serial::determine_queries(std::vector<Query>& queries, std::vector<VecN>& points, uint32_t num_points_per_query, Times *breakdown) {
    breakdown->distance_time_ms = 0.;
    breakdown->sort_time_ms = 0.;
    for (size_t i = 0; i < queries.size(); ++i) {
        const VecN query_point = queries[i].query;

        std::vector<QueryDistance> distances;

        auto distance_start = std::chrono::high_resolution_clock::now();
        for (size_t j = 0; j < points.size(); ++j) {
            QueryDistance new_qd = { 
                query_point.distance_to(points[j]),
                &points[j]
            };
            distances.push_back(new_qd);
        }
        auto distance_end = std::chrono::high_resolution_clock::now();
        breakdown->distance_time_ms += std::chrono::duration_cast<std::chrono::microseconds>(distance_end - distance_start).count() / 1000.;

        auto sort_start = std::chrono::high_resolution_clock::now();
        serial::quicksort(distances, 0, distances.size() - 1);
        auto sort_end = std::chrono::high_resolution_clock::now();
        breakdown->sort_time_ms += std::chrono::duration_cast<std::chrono::microseconds>(sort_end - sort_start).count() / 1000.;

        std::vector<QueryDistance> selected_points;
        for (uint32_t j = 0; j < num_points_per_query; ++j) {
            selected_points.push_back(distances[j]);
        }

        queries[i].points = selected_points;
    }
}

void serial::quicksort(std::vector<QueryDistance>& distances, int32_t low, int32_t high) {
    if (low >= high || low < 0)
        return;

    QueryDistance& pivot = distances[high];   

    int32_t i = low - 1;
    for (size_t j = low; j < high; ++j) {
        if (distances[j].distance <= pivot.distance) {
            ++i;
            distances[i].swap(distances[j]);
        }
    }

    ++i;
    distances[i].swap(distances[high]);

    serial::quicksort(distances, low, i - 1);
    serial::quicksort(distances, i + 1, high);
}
