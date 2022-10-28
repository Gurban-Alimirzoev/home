#pragma once
#include <iostream>
#include <string>

using namespace std::string_view_literals;

struct vtable;

extern const vtable vtable_IdentityDocument;

class IdentityDocument {
public:
    IdentityDocument()
            : vtable_ptr(&vtable_IdentityDocument),
              unique_id_(++unique_id_count_)
    {
        std::cout << "IdentityDocument::Ctor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument(const vtable* vtable_ptr)
            : vtable_ptr(vtable_ptr),
            unique_id_(++unique_id_count_) {
        std::cout << "IdentityDocument::Ctor() : "sv << unique_id_ << std::endl;
    }

    ~IdentityDocument() {
        --unique_id_count_;
        std::cout << "IdentityDocument::Dtor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument(const IdentityDocument& other)
            : vtable_ptr(&vtable_IdentityDocument),
              unique_id_(++unique_id_count_)
    {
        std::cout << "IdentityDocument::CCtor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument& operator=(const IdentityDocument&) = delete;

    void PrintID_() const {
        std::cout << "IdentityDocument::PrintID() : "sv << unique_id_ << std::endl;
    }

    void PrintID() const;

    void Delete();

    void Delete_() {
        delete this;
    }

    static void PrintUniqueIDCount() {
        std::cout << "unique_id_count_ : "sv << unique_id_count_ << std::endl;
    }

    friend vtable;

    const vtable* vtable_ptr;

protected:
    int GetID() const {
        return unique_id_;
    }

private:
    static int unique_id_count_;
    int unique_id_;
};

inline int IdentityDocument::unique_id_count_ = 0;

struct vtable
{
    void (IdentityDocument::* const PrintID)() const;
    void (IdentityDocument::* const Delete)();


    // constructor
    vtable (
            void (IdentityDocument::* const PrintID)() const,
            void (IdentityDocument::* const Delete)()
    ) : PrintID(PrintID), Delete(Delete) { }
};

inline const vtable vtable_IdentityDocument(&IdentityDocument::PrintID_, &IdentityDocument::Delete_);

inline void IdentityDocument::PrintID() const
{
    return (this->*(vtable_ptr->PrintID))();
}

inline void IdentityDocument::Delete()
{
    return (this->*(vtable_ptr->Delete))();
}
