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
    /*
     * Примерная структура программы:
     *
     * Считать JSON из stdin
     * Построить на его основе JSON базу данных транспортного справочника
     * Выполнить запросы к справочнику, находящиеся в массиве "stat_requests", построив JSON-массив
     * с ответами.
     * Вывести в stdout ответы в виде JSON
     */