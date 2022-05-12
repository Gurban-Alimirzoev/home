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
	void OutStatReader(std::string line, TransportCatalogue &cat);
private:
	std::string MakeWithoutSpace(std::string line, std::size_t symbol);
};

