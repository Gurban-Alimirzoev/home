#include "input_reader.h"

using namespace std;

namespace transport_catalogue::input
{
string_view MakeWithoutSpace(string_view line, size_t symbol)
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

namespace transport_catalogue::input
{

using namespace transport_catalogue::detail;

void InputReader::ParseCommand(string &line, TransportCatalogue& cat)
{
	if (line.substr(0, 3) == "Bus")
		lines_bus.push_back(move(line));
	else
	{
		string_view line_sv(line);
		line_sv = line_sv.substr(line_sv.find_first_not_of("Stop"));

		size_t symbol = line_sv.find(":");
		string stop_name = string(MakeWithoutSpace(line_sv, symbol));
		line_sv = line_sv.substr(symbol + 1);

		symbol = line_sv.find(",");
		string lat_str = string(MakeWithoutSpace(line_sv, symbol));
		line_sv = line_sv.substr(symbol + 1);

		symbol = line_sv.find(",");
		string lng_str = string(MakeWithoutSpace(line_sv, symbol));
		line_sv = line_sv.substr(symbol + 1);

		//double lat = 0, lng = 0;
		//from_chars(lat_str.data(), lat_str.data() + lat_str.size(), lat);
		//from_chars(lng_str.data(), lng_str.data() + lng_str.size(), lng);

		double lat = stod(lat_str);
		double lng = stod(lng_str);

		vector<pair<string, double>> associated_stops = ParseAssociatedStops(line_sv);

		buffer_distance[string(stop_name)] = associated_stops;

		cat.AddStop(move(string(stop_name)), { lat, lng });
	}
}

void InputReader::ParseDistance(TransportCatalogue& cat)
{
	for (pair<const string, vector<pair<string, double>>>& main_stop : buffer_distance)
	{
		for (const pair<string, double>& associated_stop : main_stop.second)
		{
			cat.SetEarthDistance({ move(main_stop.first), move(associated_stop.first) }, associated_stop.second);
		}
	}
}

vector<pair<string, double>> InputReader::ParseAssociatedStops(string_view line)
{
	vector<pair<string, double>> associated_stops;
	size_t symbol = line.find(",");
	while (symbol != string::npos)
	{
		auto pair_stops = ParseOneAssociatedStop(line);
		associated_stops.push_back(pair_stops);

		line = line.substr(symbol + 1);
		symbol = line.find(",");
	}

	auto pair_stops = ParseOneAssociatedStop(line);
	associated_stops.push_back(pair_stops);
	return associated_stops;
}

pair<string, double> InputReader::ParseOneAssociatedStop(string_view line)
{
	size_t symbol = line.find(",");
	string_view stop2 = MakeWithoutSpace(line, symbol);

	size_t symbol_m = stop2.find("m");
	string_view distance_str = MakeWithoutSpace(stop2, symbol_m + 1);
	//double distance = 0;
	//from_chars(distance_str.data(), distance_str.data() + distance_str.size(), distance);
	double distance = stod(string(distance_str));

	symbol_m = stop2.find("to");
	string_view assoc_stop_name = stop2.substr(symbol_m + 3, string::npos);
	assoc_stop_name = MakeWithoutSpace(assoc_stop_name, stop2.find(","));
	return { string(assoc_stop_name) , distance};
}

void InputReader::FinalizationDataRead(TransportCatalogue& cat)
{
	vector<string> result;
	for (const string& line_bus_ : lines_bus)
		{
		string_view line_bus(line_bus_);
		line_bus = line_bus.substr(line_bus.find_first_not_of("Bus"));

		size_t colon = line_bus.find(":");
		string_view bus_number = MakeWithoutSpace(line_bus, colon);
		string_view bus = line_bus.substr(colon + 1);

		size_t del = line_bus.find("-");
		if (del != string::npos)
		{
			ParseStopsInBus(result, bus, "-");
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
			ParseStopsInBus(result, bus, ">");
		}
		cat.AddBus(string(bus_number), result);
		result.clear();
	}
}

void InputReader::ParseStopsInBus(vector<string> & stops, string_view line, string symbol)
{
	size_t next_sym = line.find(symbol);;
	while (next_sym != string::npos)
	{
		next_sym = line.find(symbol);

		string one_stop = string(MakeWithoutSpace(line.substr(0, next_sym - 1), next_sym));

		stops.push_back(move(one_stop));

		line = line.substr(next_sym + 1);
	}
}

}