#include <vector>
#include <tuple>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <omp.h>

#include "VecN.hpp"
#include "auxillary.hpp"
#include "serial.hpp"
#include "omp_sections.hpp"
#include "omp_tasks.hpp"

int main(int argc, char **argv) {
    size_t dimension = 5; // Dimensionality of data points
    size_t num_points = 10; // Number of data points
    size_t num_queries = 4; // Number of neighbourhoods

    std::vector<std::tuple<size_t, size_t, size_t>> presets;
    presets.push_back({ 5, 100, 5 });
    presets.push_back({ 64, 1000000, 10000 });

    if (argc == 2) { // Preset Problem
        size_t problem_preset_idx = atoi(argv[1]) - 1;
        if (problem_preset_idx >= presets.size()) {
            fprintf(stderr, "Unknown preset problem specified\n\tSelected problem %zu of %zu problems\n", problem_preset_idx + 1, presets.size());
            return 1;
        }

        dimension = std::get<0>(presets[problem_preset_idx]);
        num_points = std::get<1>(presets[problem_preset_idx]);
        num_queries = std::get<2>(presets[problem_preset_idx]);

    } else if (argc == 4) { // Custom Problem
        dimension = atoi(argv[1]);
        num_points = atoi(argv[2]);
        num_queries = atoi(argv[3]);
    }


    printf("Problem: \n");
    printf("\tDimension (d) = %zu\n", dimension);
    printf("\tNumber of Data points (m) = %zu\n", num_points);
    printf("\tNumber of queries (n) = %zu\n", num_queries);
    printf("\tMax number of Threads = %d\n", omp_get_max_threads());

    omp_set_max_active_levels(4);
    omp_set_dynamic(1);

    // Problem Generation
    printf("\t>>> Points: Generating\n");
    VecN **points_data = new VecN*[num_points];
    for (size_t i = 0; i < num_points; ++i) {
        points_data[i] = new VecN(dimension, 0.);
        points_data[i]->randomize();
    }
    printf("\t<<< Points: Generated\n");

    printf("\t>>> Queries: Generating\n");
    VecN **queries_data = new VecN*[num_queries];
    for (size_t i = 0; i < num_queries; ++i) {
        queries_data[i] = new VecN(dimension, 0.);
        queries_data[i]->randomize();
    }
    printf("\t<<< Queries: Generated\n");
    printf("------------------------------------------------------------\n");

    VecNArray points(points_data, num_points), queries(queries_data, num_queries);

    printf("Serial:\n");
    printf("\t>>> Started k Nearest Neighbours @ k = 10\n");
    auto serial_start = std::chrono::high_resolution_clock::now();
    std::vector<QueryPoints> serial_result = serial::nearest_neigbours(points, queries, 10);
    auto serial_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> serial_duration = serial_end - serial_start;
    printf("\t<<< Finished k Nearest Neighbours @ k = 10\n");
    printf("\t||| execution time: %f ms\n", serial_duration.count());
    printf("------------------------------------------------------------\n");

    printf("OMP Sections:\n");
    printf("\t>>> Started k Nearest Neighbours @ k = 10\n");
    auto sections_start = std::chrono::high_resolution_clock::now();
    std::vector<QueryPoints> section_result = omp_sections::nearest_neigbours(points, queries, 10);
    auto sections_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> section_duration = sections_end - sections_start;
    printf("\t<<< Finished k Nearest Neighbours @ k = 10\n");
    printf("\t||| execution time: %f ms\n", section_duration.count());
    printf("------------------------------------------------------------\n");

    printf("OMP Tasks:\n");
    printf("\t>>> Started k Nearest Neighbours @ k = 10\n");
    auto tasks_start = std::chrono::high_resolution_clock::now();
    std::vector<QueryPoints> tasks_result = omp_tasks::nearest_neigbours(points, queries, 10);
    auto tasks_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> tasks_duration = tasks_end - tasks_start;
    printf("\t<<< Finished k Nearest Neighbours @ k = 10\n");
    printf("\t||| execution time: %f ms\n", tasks_duration.count());
    printf("------------------------------------------------------------\n");

    printf("Speedup: \n");
    printf("\tSections - %f\n", serial_duration.count() / section_duration.count());
    printf("\tTasks - %f\n", serial_duration.count() / tasks_duration.count());

    delete[] points_data, queries_data;

    return 0;
}
