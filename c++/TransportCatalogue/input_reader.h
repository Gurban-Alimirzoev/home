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

	TransportCatalogue ParseInputData();
private:
	std::vector<std::string> lines_stop;
	std::vector<std::string> lines_bus;


};
