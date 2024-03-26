#pragma once

#include <stdint.h>
#include <string>

struct VecN {
    uint32_t size;
    float *data;

    VecN();
    VecN(uint32_t size, float initial_value);

    float distance_to(const VecN& other) const;
    void randomize();

    void operator=(const VecN& other);
    void swap(VecN& other);

    std::string to_string();

    ~VecN();
};
