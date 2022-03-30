#pragma once

#include <cassert>
#include <initializer_list>
#include <array>
#include <stdexcept>
#include <algorithm>

#include "array_ptr.h"

using namespace std;

template <typename Type_t>
class ReserveProxyObj;

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
        : size_(other.size_), capacity_(other.size_)
    {
        // assert(other.data_ != this->data_);
        if (other.data_ != this->data_)
        {
            data_ = new ArrayPtr<Type>(other.size_);
            copy(other.begin(), other.end(), begin());
        }
    }

    SimpleVector(ReserveProxyObj<int> res)
    {
        Reserve(res.cap);
    }

    SimpleVector &operator=(const SimpleVector &rhs)
    {
        if (*this != rhs)
        {
            SimpleVector<Type> rhs_copy(rhs);
            swap(rhs_copy);
        }
        return *this;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type &item)
    {
        if (capacity_ == 0)
        {
            data_ = new ArrayPtr<Type>(1);
            data_->Get()[0] = item;
            size_++;
            capacity_++;
        }
        else if (capacity_ == size_)
        {
            ArrayPtr<Type> *data_var = new ArrayPtr<Type>(size_ * 2);
            auto begin_data_ = data_->Get();
            auto begin_data_var = data_var->Get();

            copy(begin_data_, begin_data_ + size_, begin_data_var);

            begin_data_var[size_] = item;
            data_->swap(*data_var);
            size_++;
            capacity_ = size_ * 2;
        }
        else
        {
            (*data_)[size_] = item;
            size_++;
        }
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type &value)
    {
        Iterator iter = const_cast<Iterator>(pos);
        auto dist = distance(begin(), iter);
        if (capacity_ == 0)
        {
            PushBack(value);
            return begin();
        }
        else if (capacity_ > size_)
        {
            copy(begin() + dist, end(), begin() + dist + 1u);
            (*data_)[dist] = value;
            size_++;
            return begin() + dist;
        }
        else
        {
            size_t new_size_ = size_ + 1;
            ArrayPtr<Type> *data_var = new ArrayPtr<Type>(new_size_);

            copy(begin(), begin() + dist, data_var->Get());
            copy_backward(begin() + dist, end(), data_var->Get() + new_size_);
            (*data_var)[dist] = value;

            data_->swap(*data_var);

            size_ = new_size_;
            capacity_ = new_size_;
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
        Iterator iter = const_cast<Iterator>(pos);
        auto dist = distance(begin(), iter);

        copy(begin() + dist + 1, end(), begin() + dist);

        --size_;

        return Iterator(begin() + dist);
    }

    void Reserve(size_t new_capacity)
    {
        if (capacity_ == 0)
        {
            data_ = new ArrayPtr<Type>(new_capacity);
            size_ = 0;
            capacity_ = new_capacity;
            return;
        }
        if (new_capacity > capacity_)
        {
            ArrayPtr<Type> *data_var = new ArrayPtr<Type>(new_capacity);
            // auto begin_data_ = data_->Get();
            auto begin_data_var = data_var->Get();

            copy(begin(), end(), begin_data_var);
            data_->swap(*data_var);
            capacity_ = new_capacity;
        }
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector &other) noexcept
    {
        // data_->swap(*other.data_);
        std::swap(data_, other.data_);
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
            ArrayPtr<Type> *new_array = new ArrayPtr<Type>(new_size);

            fill(new_array->Get(), new_array->Get() + new_size, Type());
            copy(data_->Get(), data_->Get() + size_, new_array->Get());

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

template <typename Type_t>
class ReserveProxyObj
{
public:
    ReserveProxyObj() noexcept = default;

    ReserveProxyObj<Type_t>(Type_t capacity_to_reserve)
        : ReserveProxyObj()
    {
        cap = capacity_to_reserve;
    }

    size_t GetSize() const
    {
        return cap;
    }

    size_t cap = 0;
};

template <>
class ReserveProxyObj<int>
{
public:
    ReserveProxyObj() noexcept = default;

    ReserveProxyObj<int>(int capacity_to_reserve)
        : ReserveProxyObj()
    {
        cap = capacity_to_reserve;
    }

    size_t GetSize() const
    {
        return cap;
    }

    size_t cap = 0;
};

template <typename Type_t>
ReserveProxyObj<Type_t> Reserve(Type_t capacity_to_reserve)
{
    return ReserveProxyObj(capacity_to_reserve);
}