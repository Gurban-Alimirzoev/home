#pragma once
#include "identity_document.h"
#include <iostream>
#include <string>
#include <ctime>

#pragma warning(disable : 4996)

using namespace std::string_view_literals;

extern const vtable vtable_Passport;

class Passport : public IdentityDocument {
public:
    Passport()
            : IdentityDocument(&vtable_Passport),
              expiration_date_(GetExpirationDate())
    {
        vtable_ptr = &vtable_Passport;
        std::cout << "Passport::Ctor()"sv << std::endl;
    }

    Passport(const Passport& other)
            : IdentityDocument(other)
            , expiration_date_(other.expiration_date_)
    {
        vtable_ptr = &vtable_Passport;
        std::cout << "Passport::CCtor()"sv << std::endl;
    }

    ~Passport() {
        std::cout << "Passport::Dtor()"sv << std::endl;
        vtable_ptr = &vtable_IdentityDocument;
    }

    void PrintID() const {
        std::cout << "Passport::PrintID() : "sv << GetID();
        std::cout << " expiration date : "sv << expiration_date_.tm_mday << "/"sv << expiration_date_.tm_mon << "/"sv
                  << expiration_date_.tm_year + 1900 << std::endl;
    }

    void PrintVisa(const std::string& country) const {
        std::cout << "Passport::PrintVisa("sv << country << ") : "sv << GetID() << std::endl;
    }
    void Delete() {
        vtable_ptr = &vtable_IdentityDocument;
        this->~Passport();
    }

private:
    const struct tm expiration_date_;

    tm GetExpirationDate() {
        time_t t = time(nullptr);
        tm exp_date = *localtime(&t);
        exp_date.tm_year += 10;
        mktime(&exp_date);
        return exp_date;
    }
};

inline const vtable vtable_Passport(
        static_cast<void (IdentityDocument::*)() const>(&Passport::PrintID), static_cast<void (IdentityDocument::*)()>(&Passport::Delete));

