#include <cassert>
#include <chrono>
#include <sstream>
#include <string_view>
#include <iostream>
#include <fstream>
#include <iostream>

#include "json_reader.h"
#include "map_renderer.h"

using namespace std::literals; 

int main() {
    JsonReader reader(std::cin, std::cout);
    reader.ParseRequests();
    reader.BaseRequests();

    transport_catalogue::RequestHandler handler(reader.GetDB());
    reader.StatRequests();

    reader.ParseRenderRequests();
    renderer::MapRenderer renderer(reader.GetSettings());


    renderer.AddPoints();
}