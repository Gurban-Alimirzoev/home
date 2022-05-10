#pragma once
#include <istream>
#include <sstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <iomanip>

#include "transport_catalogue.h"

class StatReader
{
public:
	void ParseStatReader(std::string line);

	void OutStatReader(TransportCatalogue cat);
private:
	std::vector<std::string> names;
};

