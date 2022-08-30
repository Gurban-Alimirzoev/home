#pragma once

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
    renderer::MapRenderer renderer;

    JsonReader reader(std::cin, std::cout, renderer);
    reader.ParseRequests();
    reader.ParseRenderRequests();
    renderer.SetSettings(reader.GetSettings());

    reader.BaseRequests();
    reader.StatRequests();

    reader.AddRendererElements();
    renderer.RenderMap(std::cout);
}