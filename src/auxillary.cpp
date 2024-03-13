#include "VecN.hpp"
#include "auxillary.hpp"

VecNArray::VecNArray() {
    data = nullptr;
    size = 0;
}

VecNArray::VecNArray(VecN **data, size_t size) {
    this->data = data;
    this->size = size;
}

VecNArray::~VecNArray() {
    delete[] data;
    size = 0;
}

VecN* VecNArray::operator[](const size_t index) {
    return data[index];
}