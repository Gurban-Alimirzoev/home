#include <cassert>
#include <stdexcept>
#include <algorithm>
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

    UniquePtr& operator=(const UniquePtr& ptr) = delete;

    UniquePtr(UniquePtr&& other)
    {
        if (other.Get() != ptr_)
        {
            ptr_ = (move(other.Release()));
        }
    }

    UniquePtr& operator=(nullptr_t)
    {
        delete ptr_;
        ptr_ = nullptr;
        return *this;
    }

    UniquePtr& operator=(UniquePtr&& other)
    {
        Reset(other.Release());
        return *this;
    }

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
        delete ptr_;
        ptr_ = ptr;
    }

    void Swap(UniquePtr& other)
    {
        T* var = move(other.Release());
        T* var_ = move(Release());
        other.Reset(var_);
        Reset(var);
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