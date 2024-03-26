#include <stdint.h>
#include <cmath>
#include <random>
#include <string>

#include <fmt/format.h>

#include "VecN.hpp"

VecN::VecN() {
    size = 0;
    data = nullptr;
}

VecN::VecN(uint32_t size, float initial_value) {
    this->size = size;
    this->data = new float[size];

    for (uint32_t i = 0; i < size; ++i) {
        this->data[i] = initial_value;
    }
}

float VecN::distance_to(const VecN& other) const {
    float sqr_result = 0.;

    for (uint32_t i = 0; i < size; ++i) {
        sqr_result += (data[i] - other.data[i]) * (data[i] - other.data[i]);
    }

    return std::sqrt(sqr_result);
}

void VecN::randomize() {
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<float> dist(-5,6);
    for (size_t i = 0; i < this->size; ++i)
        data[i] = dist(e2);
}

void VecN::operator=(const VecN& other) {
    size = other.size;
    data = other.data;
}

std::string VecN::to_string() {
    std::string result = "( ";

    for (uint32_t i = 0; i < size; ++i) {
        result = fmt::format("{}{} ", result, data[i]);
    }

    result = fmt::format("{})", result);
    return result;
}

VecN::~VecN() {}
