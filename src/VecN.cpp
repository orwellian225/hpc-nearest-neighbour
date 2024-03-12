#include "VecN.hpp"

#include <random>
#include <cstdlib>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>

VecN::VecN() {
    dimension = 0;
    data = nullptr;
}

VecN::VecN(size_t n, float initial_value) {
    data = new float[n];
    dimension = n;

    for (size_t i = 0; i < dimension; ++i)
        data[i] = initial_value;
}

VecN::~VecN() {
    delete[] data;
}

void VecN::randomize() {
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<float> dist(-5,6);
    for (size_t i = 0; i < this->dimension; ++i)
        data[i] = dist(e2);
}


float& VecN::operator[](const size_t index) {
    return data[index];
}

const float& VecN::operator[](const size_t index) const {
    return data[index];
}

VecN VecN::operator+(const VecN& rhs) {
    VecN result(this->dimension, 0.);

    for (size_t i = 0; i < this->dimension; ++i)
        result[i] = (*this)[i] + rhs[i];

    return result;
}

VecN VecN::operator-(const VecN& rhs) {
    VecN result(this->dimension, 0.);

    for (size_t i = 0; i < this->dimension; ++i)
        result[i] = (*this)[i] - rhs[i];

    return result;
}

float VecN::operator*(const VecN& rhs) {
    float result = 0;

    for (size_t i = 0; i < this->dimension; ++i)
        result += (*this)[i] * rhs[i];

    return result;
}

float VecN::norm1(const VecN& other) {
    float result = 0;
    for (size_t i = 0; i < this->dimension; ++i)
        result += abs((*this)[i] - other[i]);

    return result;
}

float VecN::norm2(const VecN& other) {
    float result = 0;
    for (size_t i = 0; i < this->dimension; ++i)
        result += ((*this)[i] - other[i]) * ((*this)[i] - other[i]);

    return sqrt(result);
}

void VecN::print() {
    printf("[ ");
    for (size_t i = 0; i < this->dimension; ++i)
        printf("%f ", (*this)[i]);
    printf("]\n");
}
