#pragma once

#include <cassert>
#include <initializer_list>
#include <array>
#include "array_ptr.cpp"

using namespace std;

template <typename Type>
class SimpleVector
{
public:
    using Iterator = Type *;
    using ConstIterator = const Type *;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) : size_(size)
    {
        Type var[size_];
        var.fill(NULL);
        auto varPtr = ArrayPtr(&var);
        swap(arr_vec, varPtr);
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type &value) : size_(size)
    {
        Type raw[size_];
        raw.fill(value);
        arr_vec = ArrayPtr(&raw);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) : size_(size)
    {
        Type raw[size_] = {init};
        arr_vec = ArrayPtr(&raw);
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept
    {
        // Напишите тело самостоятельно
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept
    {
        // Напишите тело самостоятельно
        return (arr_vec.Get())->max_size();
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept
    {
        return true;
    }

    // Возвращает ссылку на элемент с индексом index
    Type &operator[](size_t index) noexcept
    {
        return 
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type &operator[](size_t index) const noexcept
    {
        // Напишите тело самостоятельно
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type &At(size_t index)
    {
        // Напишите тело самостоятельно
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type &At(size_t index) const
    {
        // Напишите тело самостоятельно
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept
    {
        // Напишите тело самостоятельно
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size)
    {
        // Напишите тело самостоятельно
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept
    {
        // Напишите тело самостоятельно
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept
    {
        // Напишите тело самостоятельно
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept
    {
        // Напишите тело самостоятельно
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept
    {
        // Напишите тело самостоятельно
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept
    {
        // Напишите тело самостоятельно
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept
    {
        // Напишите тело самостоятельно
    }

private:
    size_t size_;
    Type raw[0];
    ArrayPtr<array<Type, 0u>> arr_vec;

};