#include <array>
#include <iostream>
#include <string>
#include "identity_document.h"
#include "passport.h"
#include "driving_licence.h"
#include "international_driving_licence.h"
#include "travel_pack.h"

using namespace std;

inline void PrintInfo(const IdentityDocument& doc) {
    doc.PrintID();
}

inline void PrintInfo() {
    IdentityDocument::PrintUniqueIDCount();
}

inline void PrintInfo(const Passport& pass) {
    pass.PrintVisa("France"s);
}

inline IdentityDocument PrintInfo(int i) {
    Passport pass;
    cout << "PrintInfo("sv << i << ")"sv << endl;
    pass.PrintID();
    return pass;
}

inline void PrintDrivingLicence(DrivingLicence dr_lic) {
    dr_lic.PrintID();
}

int main() {

    {
        TravelPack tt;
        //unique_ptr<IdentityDocument> test(tt);
        TravelPack t2 = tt;
    }

//    cout << "Test1"sv << endl;
//    TravelPack pass;
//    PrintInfo(pass);
//    PrintInfo(3).PrintID();
//
//    cout << "Test2"sv << endl;
//    array<IdentityDocument*, 3> docs = { (IdentityDocument*)(new TravelPack()), (IdentityDocument*)(new DrivingLicence()), (IdentityDocument*)(new TravelPack()) };
//    for (const auto* doc : docs) {
//        doc->PrintID();
//    }
//
//    for (size_t i = 0; i < docs.size(); ++i) {
//        docs[i]->Delete();
//    }
//    cout << "Test3"sv << endl;
//    array<IdentityDocument, 1> docs2 = {TravelPack()};
//    cout << "Cycle"sv << endl;
//    for (size_t i = 0; i < docs2.size(); ++i) {
//        docs2[i].PrintID();
//    }
//    cout << "PrintInfo"sv << endl;
//    PrintInfo(docs2[0]);
//    cout << "EndTest3"sv << endl;
//
//    cout << "Test4"sv << endl;
//    IdentityDocument::PrintUniqueIDCount();
//
//    cout << "Test5"sv << endl;
//    TravelPack pass2;
//    pass2.PrintUniqueIDCount();
}