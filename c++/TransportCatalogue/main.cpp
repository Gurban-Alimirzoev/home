#include <cassert>
#include <chrono>
#include <sstream>
#include <string_view>
#include <iostream>
#include <fstream>

#include "json.h"
#include "transport_catalogue.h"
#include "geo.h"
#include "json_reader.h"
#include "request_handler.h"

//using namespace json;
using namespace std::literals; 

int main() {
    std::ifstream input("D:/git/Home/c++/TransportCatalogue/input.json");
    std::string line;
    std::getline(input, line);

    JsonReader reader(line);
    reader.BaseRequests();


}
    /*
     * Примерная структура программы:
     *
     * Считать JSON из stdin
     * Построить на его основе JSON базу данных транспортного справочника
     * Выполнить запросы к справочнику, находящиеся в массиве "stat_requests", построив JSON-массив
     * с ответами.
     * Вывести в stdout ответы в виде JSON
     */