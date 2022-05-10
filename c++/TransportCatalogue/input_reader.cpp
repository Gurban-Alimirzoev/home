#include "input_reader.h"

using namespace std;

void InputReader::MakeVectorFromInput(string line)
{
	if (line[0] == 'B')
		lines_bus.push_back(line);
	else
		lines_stop.push_back(line);
}

TransportCatalogue InputReader::ParseInputData()
{
	TransportCatalogue cat;

	for (string line_stop : lines_stop)
	{
		size_t first_word_name = line_stop.find_first_not_of("Stop ");
		size_t two_point_place = line_stop.find(':');
		size_t size_name = two_point_place - 6u;
		string name_stop = line_stop.substr(first_word_name, size_name);

		string coor = line_stop.substr(two_point_place + 1);
		size_t comma = coor.find(",");

		size_t lat_begin = coor.find_first_not_of(' ');
		string lat = coor.substr(lat_begin, comma - lat_begin);
		size_t lat_end = lat.find(' ');
		lat = lat.substr(0, lat_end);


		string lng = coor.substr(comma + 1);
		size_t lng_begin = coor.find_first_not_of(' ');
		lng = lng.substr(lng_begin);
		size_t lng_end = lng.find(' ');
		lng = lng.substr(0, lng_end);

		cat.AddStop(name_stop, { atof(lat.c_str()), atof(lng.c_str()) });
	}

	for (string line_bus : lines_bus)
	{
		size_t first_space = line_bus.find(' ');
		line_bus = line_bus.substr(first_space);

		size_t number_begin = line_bus.find_first_not_of(' ');
		line_bus = line_bus.substr(number_begin);

		size_t colon = line_bus.find(':');
		string bus_number = line_bus.substr(0, colon);

		size_t bus_number_end = bus_number.find_last_not_of(' ');
		bus_number = bus_number.substr(0, bus_number_end + 1);

		string bus = line_bus.substr(colon + 1);
		size_t del = bus.find('-');

		vector<string> result;
		string one_stop;
		if (del != string::npos)
		{
			size_t next_sym = bus.find('-');
			while (next_sym != string::npos)
			{
				next_sym = bus.find('-');
				one_stop = bus.substr(0, next_sym - 1);
				size_t stop_begin = one_stop.find_first_not_of(' ');
				one_stop = one_stop.substr(stop_begin);
				size_t stop_end = one_stop.find_last_not_of(' ');
				one_stop = one_stop.substr(0, stop_end + 1);

				result.push_back(one_stop);

				bus = bus.substr(next_sym + 1);
			}
		}
		else
		{
			size_t next_sym = bus.find('>');
			while (next_sym != string::npos)
			{
				next_sym = bus.find('>');
				one_stop = bus.substr(0, next_sym - 1);
				size_t stop_begin = one_stop.find_first_not_of(' ');
				one_stop = one_stop.substr(stop_begin);
				size_t stop_end = one_stop.find_last_not_of(' ');
				one_stop = one_stop.substr(0, stop_end + 1);

				result.push_back(one_stop);

				bus = bus.substr(next_sym + 1);
			}
		}
		cat.AddBus(bus_number, result);
	}
	return cat;
}
