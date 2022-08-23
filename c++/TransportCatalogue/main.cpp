#include <cassert>
#include <chrono>
#include <sstream>
#include <string_view>
#include <iostream>
#include <fstream>
#include <iostream>

#include "json_reader.h"

using namespace std::literals; 

int main() {

    JsonReader reader(std::cin, std::cout);
    reader.Requests();
    reader.PrintAnswerJson();
}