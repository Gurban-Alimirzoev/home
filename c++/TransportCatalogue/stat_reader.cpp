#include "stat_reader.h"

using namespace std;

#include "input_reader.h"

void StatReader::OutStatReader(string line, TransportCatalogue& cat)
{
	line = line.substr(line.find_first_not_of("Bus"));

	string name = MakeWithoutSpace(line, line.find(":"));

	if (cat.FindBus(name).bus.empty())
	{
		cout << "Bus " << name << ": not found" << endl;
	}
	else
	{
		cout << "Bus " << name << ": "
			<< cat.FindBus(name).bus.size() << " stops on route, "
			<< cat.GetBusInfo(name).first << " unique stops, "
			<< std::setprecision(6) << cat.GetBusInfo(name).second << " route length"
			<< std::endl;
	}
}

string StatReader::MakeWithoutSpace(string line, size_t symbol)
{
	if (line[0] == ' ')
	{
		size_t first_space = line.find_first_not_of(' ');
		line = line.substr(first_space);
	}

	line = line.substr(0, symbol);

	if (line.back() == ' ')
	{
		size_t number_begin = line.find_last_not_of(' ');
		line = line.substr(0, number_begin + 1);
	}

	return line;
}