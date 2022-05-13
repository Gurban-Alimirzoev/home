#include "stat_reader.h"

namespace transport_catalogue::stat_
{

using namespace std;

void StatReader::OutStatReader(string line, TransportCatalogue& cat)
{

	line = line.substr(line.find_first_not_of(" "));

	if (line[0] == 'B')
	{
		ParseBusQuieries(line, cat);
	}
	else
	{
		ParseStopQuieries(line, cat);
	}
}

void StatReader::ParseBusQuieries(string line, TransportCatalogue& cat)
{
	line = line.substr(line.find_first_not_of("Bus"));

	string name = MakeWithoutSpace(line, line.find(":"));

	if (cat.FindBus(name)->bus.empty())
	{
		cout << "Bus " << name << ": not found" << endl;
	}
	else
	{
		BusInfo info = cat.GetBusInfo(name);
		cout << "Bus " << name << ": "
			<< cat.FindBus(name)->bus.size() << " stops on route, "
			<< info.unique_stop << " unique stops, "
			<< std::setprecision(6) << info.route_length << " route length, "
			<< std::setprecision(6) << info.curvature << " curvature"
			<< std::endl;
	}
}

void StatReader::ParseStopQuieries(string line, TransportCatalogue& cat)
{
	line = line.substr(line.find_first_not_of("Stop"));

	string name = MakeWithoutSpace(line, line.find(":"));
	Stop zero;

	set<string> buses_on_stop = cat.GetAllBusOnStop(name);
	if (cat.FindStop(name)->name_stop.empty())
	{
		cout << "Stop " << name << ": not found" << endl;
	}
	else if (buses_on_stop.empty())
	{
		cout << "Stop " << name << ": no buses" << endl;
	}
	else
	{
		cout << "Stop " << name << ": buses";
		for (auto i : cat.GetAllBusOnStop(name))
		{
			cout << " " << i;
		}
		cout << endl;
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

}