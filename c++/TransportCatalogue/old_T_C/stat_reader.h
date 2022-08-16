#pragma once
#include <istream>
#include <sstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <iomanip>
#include <cassert>

#include "transport_catalogue.h"

namespace transport_catalogue::stat_
{

class StatReader
{
public:
	void OutStatReader(std::string_view line, TransportCatalogue &cat);

private:
	void ParseBusQuieries(std::string_view line, TransportCatalogue& cat);

	void ParseStopQuieries(std::string_view line, TransportCatalogue& cat);
};

}