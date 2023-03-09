#include <fstream>
#include "Parse.h"

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);

    ClubManager clubManager;
    Parser parser(clubManager);
    parser.ParseInputFile(input);
    clubManager.ExecuteEvents();
    clubManager.CloseClub();
    clubManager.CalculateTotal();
}