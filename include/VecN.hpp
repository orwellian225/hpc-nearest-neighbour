#pragma once

#include <stddef.h>

struct VecN {
    size_t dimension;
    float *data;

    VecN();
    VecN(size_t n, float initial_value);
    ~VecN();

    void randomize();

    float& operator[](const size_t index);
    const float& operator[](const size_t index) const;

    VecN operator+(const VecN& rhs);
    VecN operator-(const VecN& rhs);
    float operator*(const VecN& rhs); // Dot Product

    float norm1(const VecN& other);
    float norm2(const VecN& other);

    void print();
};
