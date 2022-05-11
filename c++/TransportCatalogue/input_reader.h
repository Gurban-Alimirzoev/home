#pragma once
#include <istream>
#include <sstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <iomanip>
#include <deque>
#include <stdlib.h>

#include "transport_catalogue.h"

class InputReader
{
public:
	void MakeVectorFromInput(std::string line);

	TransportCatalogue ParseInputData(TransportCatalogue cat);

	//std::string MakeWithoutSymbol(std::string line, std::string symbol);
private:
	std::vector<std::string> lines_stop;
	std::vector<std::string> lines_bus;
	//std::vector<std::string_view> result;

};
