#pragma once
#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

extern const vtable vtable_DrivingLicence;

class DrivingLicence : public IdentityDocument {
public:
    DrivingLicence() 
    {
        std::cout << "DrivingLicence::Ctor()"sv << std::endl;
        vtable_ptr = &vtable_DrivingLicence;
    }

    DrivingLicence(const DrivingLicence& other)
            : IdentityDocument(other)
    {
        vtable_ptr = &vtable_DrivingLicence;
        std::cout << "DrivingLicence::CCtor()"sv << std::endl;
    }

    DrivingLicence(const vtable* vtable_ptr) 
    {
        std::cout << "DrivingLicence::Ctor()"sv << std::endl;
    }

    ~DrivingLicence() {
        std::cout << "DrivingLicence::Dtor()"sv << std::endl;
        vtable_ptr = &vtable_IdentityDocument;
    }

    void PrintID_() const {
        std::cout << "DrivingLicence::PrintID() : "sv << GetID() << std::endl;
    }

    void PrintID() const;

    void Delete_() {
        vtable_ptr = &vtable_IdentityDocument;
        this->~DrivingLicence();
    }

    void Delete();
};

inline const vtable vtable_DrivingLicence(
        static_cast<void (IdentityDocument::*)() const>(&DrivingLicence::PrintID_)
    , static_cast<void (IdentityDocument::*)()>(&DrivingLicence::Delete_));

inline void DrivingLicence::PrintID() const
{
    return *(vtable_ptr->PrintID)();
}

inline void DrivingLicence::Delete()
{
    return (this->*(vtable_ptr->Delete))();
}