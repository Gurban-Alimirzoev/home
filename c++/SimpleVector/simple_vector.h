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
    SimpleVector(initializer_list<Type> init)
        : size_(init.size()), capacity_(init.size())
    {
        data_ = new ArrayPtr<Type>(size_);
        std::copy(init.begin(), init.end(), begin());
    }

    SimpleVector(const SimpleVector &other)
        : size_(other.size_), capacity_(other.capacity_)
    {
        assert(other.data_ != this->data_);
        data_ = new ArrayPtr<Type>(size_);
        copy(other.begin(), other.end(), begin());
    }

    SimpleVector &operator=(const SimpleVector &rhs)
    {
        assert(*this != rhs);
        SimpleVector<Type> rhs_copy(rhs);
        swap(rhs_copy);
        return *this;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type &item)
    {
        if (capacity_ <= size_ && capacity_ > 0)
        {
            ArrayPtr<Type> *data_var = new ArrayPtr<Type>(size_ * 2);
            copy(data_->Get(), data_->Get() + size_, data_var->Get());
            (*data_var)[size_] = item;
            data_->swap(*data_var);
            size_++;
            capacity_ = size_ * 2;
        }
        else if (capacity_ == 0)
        {
            data_ = new ArrayPtr<Type>(1);
            data_->Get()[0] = item;
            size_++;
            capacity_++;
        }
        else
        {
            (*data_)[size_] = item;
            size_++;
        }
        // Напишите тело самостоятельно
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type &value)
    {
        assert(pos <= end());
        Iterator iter = const_cast<Iterator>(pos);
        auto dist = distance(begin(), iter);
        if (capacity_ == 0 || capacity_ > size_)
        {
            PushBack(value);
            return begin();
        }
        else
        {
            size_t new_size_ = size_ + 1;
            Resize(size_ * 2);

            copy_backward(begin() + dist, end(), end() + 1);
            fill(begin() + dist, begin() + dist + 1, value);

            size_ = new_size_;
            return begin() + dist;
        }
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept
    {
        if (!IsEmpty())
        {
            size_--;
        }
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos)
    {

        size_t index_pos = distance(this->begin(), const_cast<Type *>(pos));

        for (size_t i = index_pos + 1; i < size_; ++i)
        {
            data_->Get()[i - 1] = data_->Get()[i];
        }
        --size_;

        return Iterator(data_->Get() + index_pos);
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector &other) noexcept
    {
        data_->swap(*other.data_);
        std::swap(this->size_, other.size_);
        std::swap(this->capacity_, other.capacity_);
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
            throw out_of_range("Index out of range!");
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
            throw out_of_range("Index out of range!");
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

template <typename Type>
bool operator==(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return (std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) && (lhs.GetSize() == rhs.GetSize()));
}

template <typename Type>
bool operator!=(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return (!(lhs == rhs));
}

template <typename Type>
bool operator<(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return (!(lhs > rhs));
}

template <typename Type>
bool operator>(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return (rhs < lhs);
}

template <typename Type>
bool operator>=(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return !(lhs < rhs);
}