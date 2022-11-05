#include <string_view>
#include <iostream>

#include "json_reader.h"

using namespace std::literals;

int main(int argc, char* argv[]) {

    const std::string_view mode(argv[1]);

    renderer::MapRenderer renderer;
    JsonReader reader(std::cin, std::cout, renderer);
    reader.ParseRequests();
    if (mode == "make_base"sv) 
    {
        reader.ParseRenderSettings();
        renderer.SetSettings(reader.GetRenderSettings());
        reader.ParseRouteSettings();
        reader.BaseRequests();
        reader.MakeBase();
    }
    else
    {
        reader.GetBase();
        reader.AddRendererElements();
        reader.StatRequests();
        //renderer.RenderMap(std::cout);
        reader.PrintAnswerToStatRequests();
    }
}