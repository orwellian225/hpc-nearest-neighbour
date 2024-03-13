#include <vector>
#include <tuple>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "VecN.hpp"
#include "auxillary.hpp"
#include "serial.hpp"

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


    printf("Problem: \n\tDimension (d) = %zu\n\tNumber of Data points (m) = %zu\n\tNumber of neighbourhoods (n) = %zu\n", dimension, num_points, num_queries);

    // Problem Generation


    printf("------------------------------------------------------------\n");
    printf("Problem Points:\n");
    VecN **points_data = new VecN*[num_points];
    for (size_t i = 0; i < num_points; ++i) {
        points_data[i] = new VecN(dimension, 0.);
        printf("\t%zu: ", i);
        points_data[i]->randomize();
        points_data[i]->print();
    }

    printf("Problem Queries:\n");
    VecN **queries_data = new VecN*[num_queries];
    for (size_t i = 0; i < num_queries; ++i) {
        queries_data[i] = new VecN(dimension, 0.);
        printf("\t%zu: ", i);
        queries_data[i]->randomize();
        queries_data[i]->print();
    }
    printf("------------------------------------------------------------\n");
    VecNArray points, queries;
    points.data = points_data;
    points.size = num_points;
    queries.data = queries_data;
    queries.size = num_queries;

    std::vector<QueryPoints> serial_result = serial::nearest_neigbours(points, queries, 1);

    return 0;
}
