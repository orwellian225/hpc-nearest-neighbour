#pragma once

#include <vector>

struct VecNArray {
    VecN **data;
    size_t size;

    VecNArray();
    VecNArray(VecN **data, size_t size);
    ~VecNArray();

    VecN* operator[](const size_t index);
};

struct QueryPoints {
    VecN *query;
    std::vector<VecN*> points;
};