#include "stat_reader.h"

using namespace std;

void StatReader::ParseStatReader(string line)
{
	string bus_number;
	size_t last_n = line.find_last_of(' ');
	bus_number = line.substr(last_n + 1, line.size());
	names.push_back(bus_number);
}

void StatReader::OutStatReader(TransportCatalogue cat)
{
	for (string name : names)
	{
		if (cat.FindBus(name).bus.empty())
		{
			cout << "Bus " << name << ": not found" << endl;
			continue;
		}

		cout << "Bus " << name << ": "
			<< cat.FindBus(name).bus.size() << " stops on route, "
			<< cat.GetBusInfo(name).first << " unique stops, "
			<< std::setprecision(6) << cat.GetBusInfo(name).second << " route length"
			<< std::endl;
	}
}