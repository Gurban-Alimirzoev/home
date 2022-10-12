#include <stdexcept>
#include <utility>

// Исключение этого типа должно генерироватся при обращении к пустому optional
class BadOptionalAccess : public std::exception {
public:
    using exception::exception;

    virtual const char* what() const noexcept override {
        return "Bad optional access";
    }
};

template <typename T>
class Optional {
public:
    Optional() = default;
    Optional(const T& value);
    Optional(T&& value);
    Optional(const Optional& other);
    Optional(Optional&& other);

    Optional& operator=(const T& value);
    Optional& operator=(T&& rhs);
    Optional& operator=(const Optional& rhs);
    Optional& operator=(Optional&& rhs);

    ~Optional()
    {
        delete data_;
    }

    bool HasValue() const
    {
        return is_initialized_;
    }

    // Операторы * и -> не должны делать никаких проверок на пустоту Optional.
    // Эти проверки остаются на совести программиста
    T& operator*()
    {
        return this->Value();
    }
    const T& operator*() const
    {
        return this->Value();
    }
    T* operator->()
    {
        return this;
    }
    const T* operator->() const
    {
        return this->Value();
    }

    // Метод Value() генерирует исключение BadOptionalAccess, если Optional пуст
    T& Value()
    {
        if (is_initialized_)
            throw BadOptionalAccess();

        return this->Value();
    }
    const T& Value() const
    {
        if (is_initialized_)
            throw BadOptionalAccess();

        return this->Value();
    }

    void Reset()
    {
        
    }

private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) char data_[sizeof(T)];
    bool is_initialized_ = false;
};