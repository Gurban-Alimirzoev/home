#include <fstream>
#include "Parse.h"
#include "ClubManager.h"

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);

    ClubManager clubManager;
    Parser parser(clubManager);
    if (!parser.ParseInputFile(input))
        return 0;

}