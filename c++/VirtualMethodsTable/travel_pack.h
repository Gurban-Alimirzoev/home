#pragma once
#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

extern const vtable vtable_TravelPack;

class TravelPack : public IdentityDocument {
public:
    TravelPack()
            : IdentityDocument(&vtable_TravelPack)
            , identity_doc1_(new Passport())
            , identity_doc2_(new DrivingLicence())
    {
        vtable_ptr = &vtable_TravelPack;
        std::cout << "TravelPack::Ctor()"sv << std::endl;
    }

    TravelPack(const TravelPack& other)
            : IdentityDocument(other)
            , identity_doc1_(new Passport(*reinterpret_cast<Passport*>(other.identity_doc1_)))
            , identity_doc2_(new DrivingLicence(*reinterpret_cast<DrivingLicence*>(other.identity_doc2_)))
            , additional_pass_(other.additional_pass_)
            , additional_dr_licence_(other.additional_dr_licence_)
    {
        vtable_ptr = &vtable_TravelPack;
        std::cout << "TravelPack::CCtor()"sv << std::endl;
    }

    ~TravelPack() {
        identity_doc1_->Delete();
        identity_doc2_->Delete();
        std::cout << "TravelPack::Dtor()"sv << std::endl;
        vtable_ptr = &vtable_IdentityDocument;
    }

    void PrintID() const {
        identity_doc1_->PrintID();
        identity_doc2_->PrintID();
        additional_pass_.PrintID();
        additional_dr_licence_.PrintID();
    }

    void Delete() {
        vtable_ptr = &vtable_IdentityDocument;
        this->~TravelPack();
    }

private:
    IdentityDocument* identity_doc1_;
    IdentityDocument* identity_doc2_;
    Passport additional_pass_;
    DrivingLicence additional_dr_licence_;
};

inline const vtable vtable_TravelPack(
        static_cast<void (IdentityDocument::*)() const>(&TravelPack::PrintID), static_cast<void (IdentityDocument::*)()>(&TravelPack::Delete));