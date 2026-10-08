#pragma once

#include <cstddef>
#include <iostream>

class DynamicArray {
public:
    explicit DynamicArray(std::size_t size);
    DynamicArray(const DynamicArray& other);
    ~DynamicArray();

    // Копирующее присваивание реализуем в части 5. До этого запрещаем его,
    // чтобы компилятор не сгенерировал небезопасное поверхностное копирование.
    DynamicArray& operator=(const DynamicArray&) = delete;

    std::size_t size() const;
    void set(std::size_t index, int value);
    int get(std::size_t index) const;
    void append(int value);
    void add(const DynamicArray& other);
    void subtract(const DynamicArray& other);
    void print(std::ostream& out = std::cout) const;

private:
    int* data_;
    std::size_t size_;

    static void checkValue(int value);
    void checkIndex(std::size_t index) const;
};
