#include "transport_catalogue.h"
#include "geo.h"
#include "input_reader.h"
#include "stat_reader.h"

#include <sstream>
#include <cassert>
#include <iostream>
#include <fstream>

using namespace transport_catalogue;

int main() {
	std::ifstream input("e:/git/Home/c++/TransportCatalogue/tsC_case1_input.txt");

	std::string line;
	std::getline(input, line);
	int number = stoi(line);

	TransportCatalogue cat;
	input::InputReader input_reader;

	for (int i = 0; i < number; i++)
	{
		getline(input, line);
		input_reader.ParseCommand(line, cat);
	}
	input_reader.ParseDistance(cat);
	input_reader.FinalizationDataRead(cat);

	getline(input, line);
	number = stoi(line);

	stat_::StatReader stat_reader;

	for (int i = 0; i < number; i++)
	{
		getline(input, line);
		stat_reader.OutStatReader(line, cat);
	}
}