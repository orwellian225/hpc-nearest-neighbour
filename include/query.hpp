#pragma once

#include <vector>
#include <stdint.h>

#include "VecN.hpp"

struct QueryDistance {
    float distance;
    VecN* point;

    void operator= (const QueryDistance& other) {
        this->distance = other.distance;
        this->point = other.point;
    }

    void swap(QueryDistance& other) {
        float temp_distance = other.distance;
        float *temp_data = other.point->data;

        other.distance = this->distance;
        other.point->data = this->point->data;
        this->distance = temp_distance; 
        this->point->data = temp_data;
    }
};

struct Query {
    VecN query;
    std::vector<QueryDistance> points;
};
