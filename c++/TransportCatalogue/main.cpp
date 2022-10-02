#include <cassert>
#include <chrono>
#include <sstream>
#include <string_view>
#include <iostream>
#include <fstream>
#include <iostream>

#include "json_reader.h"

int main() {
    renderer::MapRenderer renderer;

    JsonReader reader(std::cin, std::cout, renderer);
    reader.ParseRequests();
    reader.ParseRenderSettings();
    reader.ParseRouteSettings();
    renderer.SetSettings(reader.GetRenderSettings());

    reader.BaseRequests();
    reader.AddRendererElements();
    reader.StatRequests();
    //renderer.RenderMap(std::cout);
    reader.PrintAnswerToStatRequests();
}