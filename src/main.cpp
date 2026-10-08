#include "DynamicArray.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <new>

int main() {
    DynamicArray a(3);
    assert(a.size() == 3);
    a.set(0, -100);
    a.set(1, 0);
    a.set(2, 100);
    assert(a.get(0) == -100);
    assert(a.get(1) == 0);
    assert(a.get(2) == 100);

    std::ostringstream output;
    a.print(output);
    assert(output.str() == "{-100, 0, 100}\n");
    std::cout << "[OK] Создание массива, set/get, вывод и границы -100..100\n";

    bool caught = false;

    try {
        a.set(0, -101);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
    assert(a.get(0) == -100);

    caught = false;
    try {
        a.set(0, 101);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
    assert(a.get(0) == -100);

    caught = false;
    try {
        a.set(3, 10);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);

    caught = false;
    try {
        a.get(3);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);

    std::cout << "[OK] Ошибки set/get и неизменность массива после ошибки\n";

    DynamicArray copy(a);
    assert(copy.size() == a.size());
    assert(copy.get(0) == a.get(0));
    copy.set(0, 42);
    assert(copy.get(0) == 42);
    assert(a.get(0) == -100);
    a.set(1, 17);
    assert(copy.get(1) == 0);

    std::cout << "[OK] Глубокое копирование: изменения независимы\n";

    const std::size_t oldSize = a.size();
    a.append(25);
    assert(a.size() == oldSize + 1);
    assert(a.get(3) == 25);
    assert(a.get(0) == -100 && a.get(1) == 17 && a.get(2) == 100);

    caught = false;
    try {
        a.append(101);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
    assert(a.size() == 4 && a.get(3) == 25);

    caught = false;
    try {
        a.append(-101);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
    assert(a.size() == 4);

    std::cout << "[OK] append: расширение, сохранение данных и проверка значений\n";

    DynamicArray shorter(2);
    shorter.set(0, 3);
    shorter.set(1, -7);
    a.add(shorter);
    assert(a.size() == 4);
    assert(a.get(0) == -97 && a.get(1) == 10);
    assert(a.get(2) == 100 && a.get(3) == 25);
    assert(shorter.get(0) == 3 && shorter.get(1) == -7);

    a.subtract(shorter);
    assert(a.size() == 4);
    assert(a.get(0) == -100 && a.get(1) == 17);
    assert(a.get(2) == 100 && a.get(3) == 25);

    DynamicArray small(2);
    small.set(0, 5);
    small.set(1, 10);

    DynamicArray longer(4);
    longer.set(0, 2);
    longer.set(1, 3);
    longer.set(2, 4);
    longer.set(3, 5);

    small.add(longer);
    assert(small.size() == 2);
    assert(small.get(0) == 7 && small.get(1) == 13);

    small.subtract(longer);
    assert(small.size() == 2);
    assert(small.get(0) == 5 && small.get(1) == 10);

    DynamicArray positive(1);
    positive.set(0, 100);

    DynamicArray hundred(1);
    hundred.set(0, 100);

    positive.add(hundred);
    assert(positive.get(0) == 200);
    positive.subtract(hundred);
    assert(positive.get(0) == 100);

    std::cout << "[OK] add/subtract: оба варианта размеров, неизменный размер и результат 200\n";

    DynamicArray empty(0);
    assert(empty.size() == 0);

    DynamicArray emptyCopy(empty);
    assert(emptyCopy.size() == 0);

    empty.add(longer);
    empty.subtract(longer);
    assert(empty.size() == 0);

    caught = false;
    try {
        empty.get(0);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);

    empty.append(-100);
    assert(empty.size() == 1 && empty.get(0) == -100);
    empty.append(100);
    assert(empty.size() == 2 && empty.get(1) == 100);

    std::cout << "[OK] Пустой массив: копирование, арифметика, индексы и append\n";

    bool memoryErrorCaught = false;

    try {
        DynamicArray huge(
            std::numeric_limits<std::size_t>::max()
        );
    } catch (const std::bad_alloc& e) {
        memoryErrorCaught = true;
        std::cout << "[OK] bad_alloc: " << e.what() << '\n';
    }

    assert(memoryErrorCaught);

    std::cout << "Все проверки части 2 прошли успешно.\n";

    return 0;
}