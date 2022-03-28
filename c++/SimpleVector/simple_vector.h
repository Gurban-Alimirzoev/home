#pragma once

#include <cassert>
#include <initializer_list>
#include <array>
#include <stdexcept>
#include <algorithm>
#include "array_ptr.h"

using namespace std;

template <typename Type>
class SimpleVector
{
public:
    using Iterator = Type *;
    using ConstIterator = const Type *;

    SimpleVector() noexcept = default;
    ~SimpleVector()
    {
        delete data_;
    }

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size)
        : size_(size), capacity_(size)
    {
        data_ = new ArrayPtr<Type>(size);
        fill(begin(), end(), Type());
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type &value)
        : size_(size), capacity_(size)
    {
        data_ = new ArrayPtr<Type>(size);
        fill(begin(), end(), value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init)
        : size_(init.size()), capacity_(init.size())
    {

        data_ = new ArrayPtr<Type>(size_);
        std::copy(init.begin(), init.end(), begin());
    }

    SimpleVector &operator=(const SimpleVector &rhs)
    {
        assert(*this != rhs);
        SimpleVector<Type> rhs_copy(rhs);
        swap(rhs_copy);
        return *this;
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept
    {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept
    {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept
    {
        return size_ == 0u;
    }

    // Возвращает ссылку на элемент с индексом index
    Type &operator[](size_t index) noexcept
    {
        return *(data_->Get() + index);
    }
    // Возвращает константную ссылку на элемент с индексом index
    const Type &operator[](size_t index) const noexcept
    {
        return *(data_->Get() + index);
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type &At(size_t index)
    {
        if (!(index >= size_))
        {
            return *(data_->Get() + index);
        }
        else
        {
            throw out_of_range("Exception: Index out of range!");
        }
        return *data_->Get();
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type &At(size_t index) const
    {
        if (!(index >= size_))
        {
            return *(data_->Get() + index);
        }
        else
        {
            throw out_of_range("Exception: Index out of range!");
        }
        return *data_->Get();
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept
    {
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size)
    {
        if (new_size > size_)
        {
            auto new_array = new ArrayPtr<Type>(new_size);

            fill(new_array->Get(), new_array->Get() + new_size, Type());
            copy(begin(), begin() + size_, new_array->Get());

            data_ = new_array;

            size_ = new_size;
            capacity_ = new_size;
        }
        else
        {
            size_ = new_size;
        }
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept
    {
        return (size_ == 0) ? Iterator() : Iterator(data_->Get());
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept
    {
        return (size_ == 0) ? Iterator() : Iterator(data_->Get() + size_);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept
    {
        return (size_ == 0) ? ConstIterator() : ConstIterator(data_->Get());
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept
    {
        return (size_ == 0) ? ConstIterator() : ConstIterator(data_->Get() + size_);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept
    {
        return (size_ == 0) ? ConstIterator() : ConstIterator(data_->Get());
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept
    {
        return (size_ == 0) ? ConstIterator() : ConstIterator(data_->Get() + size_);
    }

private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    ArrayPtr<Type> *data_ = nullptr;
};