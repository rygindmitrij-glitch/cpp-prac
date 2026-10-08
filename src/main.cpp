#include "DynamicArray.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <new>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>

int main() {
    DynamicArray<int> a(3);
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

    DynamicArray<int> copy(a);
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

    DynamicArray<int> shorter(2);
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

    DynamicArray<int> small(2);
    small.set(0, 5);
    small.set(1, 10);

    DynamicArray<int> longer(4);
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

    DynamicArray<int> positive(1);
    positive.set(0, 100);

    DynamicArray<int> hundred(1);
    hundred.set(0, 100);

    positive.add(hundred);
    assert(positive.get(0) == 200);
    positive.subtract(hundred);
    assert(positive.get(0) == 100);
    std::cout << "[OK] add/subtract: размеры, неизменный размер и результат 200\n";

    DynamicArray<int> empty(0);
    assert(empty.size() == 0);

    DynamicArray<int> emptyCopy(empty);
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
    std::cout << "[OK] Пустой массив и append\n";

    bool memoryErrorCaught = false;
    try {
        DynamicArray<int> huge(std::numeric_limits<std::size_t>::max());
    } catch (const std::bad_alloc&) {
        memoryErrorCaught = true;
    }
    assert(memoryErrorCaught);
    std::cout << "[OK] bad_alloc\n";

    DynamicArray<double> fractions(2);
    fractions.set(0, -250.5);
    fractions.set(1, 300.25);
    assert(fractions.get(0) == -250.5);
    fractions.append(1000.5);
    assert(fractions.size() == 3);
    assert(fractions.get(2) == 1000.5);
    std::cout << "[OK] Шаблон для double без целочисленных ограничений\n";

    DynamicArray<unsigned int> unsignedNumbers(1);
    unsignedNumbers.set(0, 100);
    caught = false;
    try {
        unsignedNumbers.set(0, 101);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
    std::cout << "[OK] Проверка диапазона для unsigned int\n";

    DynamicArray<std::string> words(2);
    words.set(0, "Привет");
    words.set(1, "мир");
    words.append("C++");
    assert(words.size() == 3);
    assert(words.get(0) == "Привет");
    assert(words.get(2) == "C++");

    std::ostringstream stringOutput;
    stringOutput << words;
    assert(stringOutput.str() == "{Привет, мир, C++}");
    std::cout << "[OK] Шаблон для std::string и operator<<\n";

    DynamicArray<double> left(2);
    left.set(0, 0.0);
    left.set(1, 0.0);

    DynamicArray<double> right(2);
    right.set(0, 3.0);
    right.set(1, 4.0);

    assert(std::abs(left.distance(right) - 5.0) < 1e-12);
    assert(std::abs(right.distance(left) - 5.0) < 1e-12);
    assert(left.distance(left) == 0.0);
    std::cout << "[OK] Евклидово расстояние для чисел\n";

    caught = false;
    try {
        left.distance(fractions);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
    std::cout << "[OK] Разные размеры при вычислении расстояния\n";

    caught = false;
    try {
        words.distance(words);
    } catch (const std::bad_typeid&) {
        caught = true;
    }
    assert(caught);
    std::cout << "[OK] std::bad_typeid для строкового массива\n";

    DynamicArray<int> indexed(3);
    indexed[0] = -100;
    indexed[1] = 27;
    indexed[2] = 100;
    assert(indexed.get(0) == -100 && indexed.get(1) == 27 && indexed.get(2) == 100);
    const DynamicArray<int>& constIndexed = indexed;
    assert(constIndexed[0] == -100 && constIndexed[1] == 27);

    caught = false;
    try {
        indexed[1] = 101;
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught && indexed.get(1) == 27);

    caught = false;
    try {
        indexed[3] = 7;
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);

    caught = false;
    try {
        constIndexed[3];
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);

    indexed[1] = indexed[0];
    assert(indexed.get(1) == -100);
    std::ostringstream proxyStream;
    proxyStream << indexed[2];
    assert(proxyStream.str() == "100");
    std::cout << "[OK] operator[]: чтение, запись, проверка диапазона и индекса\n";

    DynamicArray<int> equalArray(3);
    equalArray.set(0, -100);
    equalArray.set(1, -100);
    equalArray.set(2, 100);
    assert(indexed == equalArray);
    assert(!(indexed != equalArray));
    equalArray.set(2, 0);
    assert(indexed != equalArray);
    assert(!(indexed == equalArray));
    assert(indexed != small);
    assert(emptyCopy == DynamicArray<int>(0));
    assert(words == words);
    std::cout << "[OK] operator== и operator!=: элементы, размер и строки\n";

    DynamicArray<int> combined(4);
    combined.set(0, 100);
    combined.set(1, 2);
    combined.set(2, 3);
    combined.set(3, 4);
    DynamicArray<int> few(2);
    few.set(0, 100);
    few.set(1, -2);
    combined += few;
    assert(combined.size() == 4);
    assert(combined.get(0) == 200 && combined.get(1) == 0);
    assert(combined.get(2) == 3 && combined.get(3) == 4);
    combined -= few;
    assert(combined.get(0) == 100 && combined.get(1) == 2);
    assert(combined.get(2) == 3 && combined.get(3) == 4);
    few += combined;
    assert(few.size() == 2 && few.get(0) == 200 && few.get(1) == 0);
    few -= combined;
    assert(few.get(0) == 100 && few.get(1) == -2);
    std::cout << "[OK] operator+= и operator-=: массивы разной длины\n";

    combined += 10;
    assert(combined.get(0) == 110 && combined.get(1) == 12);
    assert(combined.get(2) == 13 && combined.get(3) == 14);
    combined -= 10;
    assert(combined.get(0) == 100 && combined.get(1) == 2);
    assert(combined.get(2) == 3 && combined.get(3) == 4);
    DynamicArray<double> scalarDoubles(2);
    scalarDoubles[0] = 200.25;
    scalarDoubles[1] = -150.5;
    scalarDoubles += 0.5;
    assert(scalarDoubles.get(0) == 200.75 && scalarDoubles.get(1) == -150.0);
    scalarDoubles -= 0.5;
    assert(scalarDoubles.get(0) == 200.25 && scalarDoubles.get(1) == -150.5);
    std::cout << "[OK] operator+= и operator-=: числовые скаляры\n";

    caught = false;
    try {
        words += words;
    } catch (const std::bad_typeid&) {
        caught = true;
    }
    assert(caught);
    caught = false;
    try {
        words += std::string("!");
    } catch (const std::bad_typeid&) {
        caught = true;
    }
    assert(caught);
    std::cout << "[OK] Нечисловая арифметика вызывает std::bad_typeid\n";

    DynamicArray<int> iterated(3);
    int nextValue = 1;
    for (auto& element : iterated) {
        element = nextValue++;
    }
    assert(iterated.get(0) == 1 && iterated.get(1) == 2 && iterated.get(2) == 3);
    int sum = 0;
    for (const auto& element : static_cast<const DynamicArray<int>&>(iterated)) {
        sum += element;
    }
    assert(sum == 6);
    auto iterator = iterated.begin();
    *iterator = 10;
    iterator++;
    *iterator = 20;
    assert(iterated.get(0) == 10 && iterated.get(1) == 20);
    auto secondIterator = iterated.begin();
    ++secondIterator;
    auto copiedIterator = iterated.begin();
    copiedIterator = secondIterator;
    assert(static_cast<int>(*copiedIterator) == 20);
    assert(iterated.get(0) == 10 && iterated.get(1) == 20);
    for (auto& word : words) {
        std::ostringstream s;
        s << word;
        assert(!s.str().empty());
    }
    caught = false;
    try {
        for (auto& element : iterated) {
            element = 101;
        }
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
    assert(iterated.get(0) == 10 && iterated.get(1) == 20 && iterated.get(2) == 3);
    DynamicArray<int> zeroLength(0);
    assert(zeroLength.begin() == zeroLength.end());
    const DynamicArray<int>& constZeroLength = zeroLength;
    assert(constZeroLength.begin() == constZeroLength.end());
    std::cout << "[OK] begin/end: изменяемые и константные итераторы\n";

    std::cout << "Все проверки части 4 прошли успешно.\n";
    return 0;
}
