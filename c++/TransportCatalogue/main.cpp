#include <cassert>
#include <chrono>
#include <sstream>
#include <string_view>

#include "json.h"
#include "transport_catalogue.h"
#include "geo.h"
#include "json_reader.h"

using namespace json;
using namespace std::literals; 

int main() {



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