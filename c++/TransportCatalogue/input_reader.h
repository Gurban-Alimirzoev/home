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
#include <charconv>

#include "transport_catalogue.h"

namespace transport_catalogue::input
{
std::string_view MakeWithoutSpace(std::string_view line, std::size_t symbol);
}

namespace transport_catalogue::input
{

class InputReader
{
public:
	void ParseCommand(std::string& line, TransportCatalogue &cat);

	void ParseDistance(TransportCatalogue& cat);

	void FinalizationDataRead(TransportCatalogue &cat);

private:
	std::vector<std::string> lines_bus;

	std::unordered_map < std::string, std::vector<std::pair<std::string, double>>> buffer_distance;

	std::vector<std::pair<std::string, double>> ParseAssociatedStops(std::string_view line);

	std::pair<std::string, double> ParseOneAssociatedStop(std::string_view line);

	void ParseStopsInBus(std::vector<std::string> &stops, std::string_view line, std::string symbol);

};

}