#include <vector>
#include <tuple>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "VecN.hpp"

int main(int argc, char **argv) {
    size_t dimension = 5; // Dimensionality of data points
    size_t num_data_points = 10; // Number of data points
    size_t num_neighbourhoods = 4; // Number of neighbourhoods

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
        num_data_points = std::get<1>(presets[problem_preset_idx]);
        num_neighbourhoods = std::get<2>(presets[problem_preset_idx]);

    } else if (argc == 4) { // Custom Problem
        dimension = atoi(argv[1]);
        num_data_points = atoi(argv[2]);
        num_neighbourhoods = atoi(argv[3]);
    }


    printf("Problem: \n\tDimension (d) = %zu\n\tNumber of Data points (m) = %zu\n\tNumber of neighbourhoods (n) = %zu\n", dimension, num_data_points, num_neighbourhoods);

    // Problem Generation
    printf("------------------------------------------------------------\n");
    printf("Problem Data:\n");
    VecN **data = new VecN*[num_data_points];
    for (size_t i = 0; i < num_data_points; ++i) {
        data[i] = new VecN(dimension, 0.);
        printf("%zu: ", i);
        data[i]->randomize();
        data[i]->print();
    }
    printf("------------------------------------------------------------\n");

    delete[] data;
    return 0;
}
