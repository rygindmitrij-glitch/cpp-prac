#include "DynamicArray.h"

#include <algorithm>
#include <stdexcept>

DynamicArray::DynamicArray(std::size_t size)
    : data_(size ? new int[size]{} : nullptr), size_(size) {}

DynamicArray::DynamicArray(const DynamicArray& other)
    : data_(other.size_ ? new int[other.size_] : nullptr), size_(other.size_) {
    if (size_ != 0) {
        std::copy(other.data_, other.data_ + size_, data_);
    }
}

DynamicArray::~DynamicArray() {
    delete[] data_;
}

std::size_t DynamicArray::size() const {
    return size_;
}

void DynamicArray::checkValue(int value) {
    if (value < -100 || value > 100) {
        throw std::invalid_argument("Значение должно быть в диапазоне [-100, 100]");
    }
}

void DynamicArray::checkIndex(std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Индекс вне границ массива");
    }
}

void DynamicArray::set(std::size_t index, int value) {
    checkIndex(index);
    checkValue(value);
    data_[index] = value;
}

int DynamicArray::get(std::size_t index) const {
    checkIndex(index);
    return data_[index];
}

void DynamicArray::append(int value) {
    checkValue(value);
    int* expanded = new int[size_ + 1];
    if (size_ != 0) {
        std::copy(data_, data_ + size_, expanded);
    }
    expanded[size_] = value;
    delete[] data_;
    data_ = expanded;
    ++size_;
}

void DynamicArray::add(const DynamicArray& other) {
    for (std::size_t i = 0; i < std::min(size_, other.size_); ++i) {
        data_[i] += other.data_[i];
    }
}

void DynamicArray::subtract(const DynamicArray& other) {
    for (std::size_t i = 0; i < std::min(size_, other.size_); ++i) {
        data_[i] -= other.data_[i];
    }
}

void DynamicArray::print(std::ostream& out) const {
    out << '{';
    for (std::size_t i = 0; i < size_; ++i) {
        if (i != 0) out << ", ";
        out << data_[i];
    }
    out << "}\n";
}
