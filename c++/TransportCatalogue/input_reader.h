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
#include <execution>

#include "transport_catalogue.h"

namespace transport_catalogue::input
{

class InputReader
{
public:
	void ParseStopOrSaveBus(std::string &line, TransportCatalogue &cat);

	void ParseBus(TransportCatalogue &cat);

private:
	std::vector<std::string> lines_bus;

	std::string MakeWithoutSpace(std::string line, std::size_t symbol);

	std::vector<std::pair<std::string, std::string>> ParseAssocStops(std::string line);

	std::pair<std::string, std::string> ParseOneAssocStop(std::string line);

};

}