#include "document.h"
//#include "document.cpp"

#include "search_server.h"
//#include "search_server.cpp"

#include "string_processing.h"
//#include "string_processing.cpp"

#include "paginator.h"

#include "read_input_functions.h"
//#include "read_input_functions.cpp"

#include "request_queue.h"
//#include "request_queue.cpp"

#include "remove_duplicates.h"
//#include "remove_duplicates.cpp"

#include "test_example_functions.h"
//#include "test_example_functions.cpp"

#include "log_duration.h"

//#include "process_queries.h"

int main() {
    TestSearchServer();
    /*SearchServer search_server("and with"s);

    int id = 0;
    for (
        const std::string& text : {
            "funny pet and nasty rat"s,
            "funny pet with curly hair"s,
            "funny pet and not very nasty rat"s,
            "pet with rat and rat and rat"s,
            "nasty rat with curly hair"s,
        }
    ) {
        search_server.AddDocument(++id, text, DocumentStatus::ACTUAL, {1, 2});
    }

    const std::vector<std::string> queries = {
        "nasty rat -not"s,
        "not very funny nasty pet"s,
        "curly hair"s
    };
    id = 0;
    for (
        const auto& documents : ProcessQueries(search_server, queries)
    ) {
        std::cout << documents.size() << " documents for query ["s << queries[id++] << "]"s << std::endl;
    }

    return 0;*/
}