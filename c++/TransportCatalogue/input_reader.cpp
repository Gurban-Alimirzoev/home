#include "input_reader.h"

namespace transport_catalogue::input
{

using namespace std;

void InputReader::ParseStopOrSaveBus(string &line, TransportCatalogue& cat)
{
	if (line[0] == 'B')
		lines_bus.push_back(move(line));
	else
	{
		line = line.substr(line.find_first_not_of("Stop"));
		size_t symbol = line.find(":");
		string stop_name = MakeWithoutSpace(line, symbol);

		line = line.substr(symbol + 1);
		symbol = line.find(",");
		string lat = MakeWithoutSpace(line, symbol);

		line = line.substr(symbol + 1);
		symbol = line.find(",");
		string lng = MakeWithoutSpace(line, symbol);

		line = line.substr(symbol + 1);
		vector<pair<string, string>> associated_stops = ParseAssocStops(line);

		cat.AddStop(move(stop_name), { atof(lat.c_str()), atof(lng.c_str()) }, associated_stops);
	}
}

vector<pair<string, string>> InputReader::ParseAssocStops(string line)
{
	vector<pair<string, string>> associated_stops;
	size_t symbol = line.find(",");
	while (symbol != string::npos)
	{
		auto pair_stops = ParseOneAssocStop(line);
		associated_stops.push_back(pair_stops);

		line = line.substr(symbol + 1);
		symbol = line.find(",");
	}

	auto pair_stops = ParseOneAssocStop(line);
	associated_stops.push_back(pair_stops);
	return associated_stops;
}

pair<string, string> InputReader::ParseOneAssocStop(string line)
{
	size_t symbol = line.find(",");
	string stop2 = MakeWithoutSpace(line, symbol);

	size_t symbol_m = stop2.find("m");
	string distance = MakeWithoutSpace(stop2, symbol_m + 1);

	symbol_m = stop2.find("to");
	string assoc_stop_name = stop2.substr(symbol_m + 3, string::npos);
	assoc_stop_name = MakeWithoutSpace(assoc_stop_name, stop2.find(","));
	return { assoc_stop_name , distance };
}

void InputReader::ParseBus(TransportCatalogue& cat)
{
	vector<string> result;
	for_each(
		lines_bus.begin(), lines_bus.end(),
		[this, &cat, &result](string line_bus)
		{
			line_bus = line_bus.substr(line_bus.find_first_not_of("Bus"));

			size_t colon = line_bus.find(":");
			string bus_number = MakeWithoutSpace(line_bus, colon);

			string bus = line_bus.substr(colon + 1);

			size_t del = bus.find('-');
			string one_stop;

			if (del != string::npos)
			{
				size_t next_sym = bus.find('-');
				while (next_sym != string::npos)
				{
					next_sym = bus.find("-");

					one_stop = MakeWithoutSpace(bus.substr(0, next_sym - 1), next_sym);

					result.push_back(move(one_stop));

					bus = bus.substr(next_sym + 1);
				}

				vector<string> reverse_result(result.begin(), result.end());
				reverse_result.pop_back();
				reverse(reverse_result.begin(), reverse_result.end());

				for (string stop_rev : reverse_result)
				{
					result.push_back(move(stop_rev));
				}

			}
			else
			{
				size_t next_sym = bus.find(">");
				while (next_sym != string::npos)
				{
					next_sym = bus.find(">");

					one_stop = MakeWithoutSpace(bus.substr(0, next_sym - 1), next_sym);

					result.push_back(move(one_stop));

					bus = bus.substr(next_sym + 1);
				}
			}
			cat.AddBus(move(bus_number), move(result));
			result.clear();
		}
	);
}


string InputReader::MakeWithoutSpace(string line, size_t symbol)
{
	if (line[0] == ' ')
	{
		size_t first_space = line.find_first_not_of(' ');
		line = line.substr(first_space);
	}

	line = line.substr(0, symbol - 1);

	if (line.back() == ' ')
	{
		size_t number_begin = line.find_last_not_of(' ');
		line = line.substr(0, number_begin + 1);
	}

	return line;
}

}