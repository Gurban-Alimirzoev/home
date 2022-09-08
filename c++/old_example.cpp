#include <cassert>
#include <stdexcept>
#include <cstddef>  // нужно для nullptr_t

using namespace std;

// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
private:
    T* ptr_ = nullptr;
public:
    UniquePtr() = default;

    explicit UniquePtr(T* ptr)
        : ptr_(ptr)
    {
    }

    UniquePtr(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) = default;

    UniquePtr& operator=(const UniquePtr& ptr)
    {
        ptr_ = ptr;
        return ptr_;
    }
    UniquePtr& operator=(nullptr_t)
    {
        ~UniquePtr();
        ptr_ = nullptr;
    }
    UniquePtr& operator=(UniquePtr&& other) = default;

    ~UniquePtr()
    {
        delete ptr_;
    }

    T& operator*() const
    {
        if (!ptr_) {
            throw logic_error("Unique ptr is null");
        }
        return *ptr_;
    }

    T* operator->() const
    {
        if (!ptr_) {
            throw logic_error("Unique ptr is null");
        }
        return ptr_;
    }

    T* Release()
    {
        T* p = ptr_;
        ptr_ = nullptr;
        return p;
    }
    void Reset(T* ptr)
    {
        *ptr_ = *ptr;
    }
    void Swap(UniquePtr& other)
    {
        T* var = other;
        swap(ptr_, var);
    }
    T* Get() const
    {
        return ptr_;
    }
};

struct Item {
    static int counter;
    int value;
    Item(int v = 0)
        : value(v)
    {
        ++counter;
    }
    Item(const Item& other)
        : value(other.value)
    {
        ++counter;
    }
    ~Item() {
        --counter;
    }
};

int Item::counter = 0;

void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);

        ptr.Reset(new Item);
        assert(Item::counter == 1);
    }
    assert(Item::counter == 0);

    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);

        auto rawPtr = ptr.Release();
        assert(Item::counter == 1);

        delete rawPtr;
        assert(Item::counter == 0);
    }
    assert(Item::counter == 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    assert(ptr.Get()->value == 42);
    assert((*ptr).value == 42);
    assert(ptr->value == 42);
}

int main() {
    TestLifetime();
    TestGetters();
}