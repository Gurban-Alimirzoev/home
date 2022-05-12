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

class InputReader
{
public:
	void ParseStopOrSaveBus(std::string &line, TransportCatalogue &cat);

	void ParseBus(TransportCatalogue &cat);

private:
	std::vector<std::string> lines_bus;

	std::string MakeWithoutSpace(std::string line, std::size_t symbol);

};
