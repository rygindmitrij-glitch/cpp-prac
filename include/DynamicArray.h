#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>

template <typename T>
class DynamicArray {
public:
    explicit DynamicArray(std::size_t size)
        : data_(size ? new T[size]{} : nullptr), size_(size) {}

    DynamicArray(const DynamicArray& other)
        : data_(other.size_ ? new T[other.size_] : nullptr), size_(other.size_) {
        if (size_ != 0) {
            std::copy(other.data_, other.data_ + size_, data_);
        }
    }

    ~DynamicArray() {
        delete[] data_;
    }

    DynamicArray& operator=(const DynamicArray&) = delete;

    std::size_t size() const {
        return size_;
    }

    void set(std::size_t index, const T& value) {
        checkIndex(index);
        checkValue(value);
        data_[index] = value;
    }

    const T& get(std::size_t index) const {
        checkIndex(index);
        return data_[index];
    }

    class Iterator;

    class ElementProxy {
    public:
        ElementProxy(DynamicArray* owner, std::size_t index)
            : owner_(owner), index_(index) {}

        ElementProxy(const ElementProxy&) = default;

        ElementProxy& operator=(const T& value) {
            owner_->set(index_, value);
            return *this;
        }

        ElementProxy& operator=(const ElementProxy& other) {
            return *this = static_cast<const T&>(other);
        }

        operator const T&() const {
            return owner_->get(index_);
        }

        friend std::ostream& operator<<(std::ostream& out, const ElementProxy& element) {
            return out << element.owner_->get(element.index_);
        }

    private:
        friend class Iterator;
        DynamicArray* owner_;
        std::size_t index_;
    };

    ElementProxy operator[](std::size_t index) {
        checkIndex(index);
        return ElementProxy(this, index);
    }

    const T& operator[](std::size_t index) const {
        return get(index);
    }

    bool operator==(const DynamicArray& other) const {
        if (size_ != other.size_) {
            return false;
        }
        for (std::size_t i = 0; i < size_; ++i) {
            if (!(data_[i] == other.data_[i])) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const DynamicArray& other) const {
        return !(*this == other);
    }

    DynamicArray& operator+=(const DynamicArray& other) {
        if constexpr (std::is_arithmetic_v<T>) {
            for (std::size_t i = 0; i < std::min(size_, other.size_); ++i) {
                data_[i] += other.data_[i];
            }
        } else {
            throw std::bad_typeid();
        }
        return *this;
    }

    DynamicArray& operator-=(const DynamicArray& other) {
        if constexpr (std::is_arithmetic_v<T>) {
            for (std::size_t i = 0; i < std::min(size_, other.size_); ++i) {
                data_[i] -= other.data_[i];
            }
        } else {
            throw std::bad_typeid();
        }
        return *this;
    }

    DynamicArray& operator+=(const T& scalar) {
        if constexpr (std::is_arithmetic_v<T>) {
            for (std::size_t i = 0; i < size_; ++i) {
                data_[i] += scalar;
            }
        } else {
            throw std::bad_typeid();
        }
        return *this;
    }

    DynamicArray& operator-=(const T& scalar) {
        if constexpr (std::is_arithmetic_v<T>) {
            for (std::size_t i = 0; i < size_; ++i) {
                data_[i] -= scalar;
            }
        } else {
            throw std::bad_typeid();
        }
        return *this;
    }

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = void;
        using reference = ElementProxy&;

        Iterator(DynamicArray* owner, std::size_t index)
            : owner_(owner), index_(index), proxy_(owner, index) {}

        Iterator(const Iterator&) = default;

        Iterator& operator=(const Iterator& other) {
            if (this != &other) {
                owner_ = other.owner_;
                index_ = other.index_;
                proxy_.owner_ = owner_;
                proxy_.index_ = index_;
            }
            return *this;
        }

        ElementProxy& operator*() const {
            return proxy_;
        }

        Iterator& operator++() {
            ++index_;
            proxy_.index_ = index_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator old = *this;
            ++(*this);
            return old;
        }

        bool operator==(const Iterator& other) const {
            return owner_ == other.owner_ && index_ == other.index_;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

    private:
        DynamicArray* owner_;
        std::size_t index_;
        mutable ElementProxy proxy_;
    };

    Iterator begin() {
        return Iterator(this, 0);
    }

    Iterator end() {
        return Iterator(this, size_);
    }

    const T* begin() const {
        return data_;
    }

    const T* end() const {
        return data_ ? data_ + size_ : nullptr;
    }

    const T* cbegin() const {
        return begin();
    }

    const T* cend() const {
        return end();
    }

    void append(const T& value) {
        checkValue(value);
        T* expanded = new T[size_ + 1]{};
        try {
            if (size_ != 0) {
                std::copy(data_, data_ + size_, expanded);
            }
            expanded[size_] = value;
        } catch (...) {
            delete[] expanded;
            throw;
        }
        delete[] data_;
        data_ = expanded;
        ++size_;
    }

    void add(const DynamicArray& other) {
        *this += other;
    }

    void subtract(const DynamicArray& other) {
        *this -= other;
    }

    void print(std::ostream& out = std::cout) const {
        out << *this << '\n';
    }

    double distance(const DynamicArray& other) const {
        if constexpr (std::is_arithmetic_v<T>) {
            if (size_ != other.size_) {
                throw std::invalid_argument("Размеры массивов должны совпадать");
            }
            long double sum = 0;
            for (std::size_t i = 0; i < size_; ++i) {
                const long double delta = static_cast<long double>(data_[i])
                    - static_cast<long double>(other.data_[i]);
                sum += delta * delta;
            }
            return static_cast<double>(std::sqrt(sum));
        } else {
            throw std::bad_typeid();
        }
    }

    friend std::ostream& operator<<(std::ostream& out, const DynamicArray& array) {
        out << '{';
        for (std::size_t i = 0; i < array.size_; ++i) {
            if (i != 0) {
                out << ", ";
            }
            out << array.data_[i];
        }
        return out << '}';
    }

private:
    T* data_;
    std::size_t size_;

    static void checkValue(const T& value) {
        if constexpr (std::is_integral_v<T>) {
            const long double number = static_cast<long double>(value);
            if (number < -100 || number > 100) {
                throw std::invalid_argument("Значение должно быть в диапазоне [-100, 100]");
            }
        }
    }

    void checkIndex(std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Индекс вне границ массива");
        }
    }
};
