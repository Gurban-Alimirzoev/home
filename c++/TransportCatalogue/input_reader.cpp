#include "input_reader.h"

using namespace std;

void InputReader::ParseStopOrSaveBus(string &line, TransportCatalogue& cat)
{
	if (line[0] == 'B')
		lines_bus.push_back(move(line));
	else
	{
		line = line.substr(line.find_first_not_of("Stop"));

		size_t colon = line.find(":");
		string stop_name = MakeWithoutSpace(line, colon);

		string coor = line.substr(colon + 1);

		size_t comma = coor.find(",");
		string lat = MakeWithoutSpace(coor, comma);

		string lng = MakeWithoutSpace(coor.substr(comma + 1), coor.substr(comma + 1).size());

		cat.AddStop(move(stop_name), { atof(lat.c_str()), atof(lng.c_str()) });
	}
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