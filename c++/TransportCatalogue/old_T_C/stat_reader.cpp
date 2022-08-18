#include "stat_reader.h"
#include "input_reader.h"

namespace transport_catalogue::stat_
{

using namespace std;
using namespace input;

void StatReader::OutStatReader(string_view line, TransportCatalogue& cat)
{

	line = line.substr(line.find_first_not_of(" "));

	if (line.substr(0, 3) == "Bus")
		ParseBusQuieries(line, cat);
	else
		ParseStopQuieries(line, cat);
}

void StatReader::ParseBusQuieries(string_view line, TransportCatalogue& cat)
{
	line = line.substr(line.find_first_not_of("Bus"));

	string_view name = MakeWithoutSpace(line, line.find(":"));

	if (cat.FindBus(name) == nullptr)
		cout << "Bus " << name << ": not found" << endl;
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

void StatReader::ParseStopQuieries(string_view line, TransportCatalogue& cat)
{
	line = line.substr(line.find_first_not_of("Stop"));

	string_view name = MakeWithoutSpace(line, line.find(":"));

	set<string> buses_on_stop = cat.GetAllBusOnStop(name);

	if (cat.FindStop(name) == nullptr)
		cout << "Stop " << name << ": not found" << endl;
	else if (buses_on_stop.empty())
		cout << "Stop " << name << ": no buses" << endl;
	else
	{
		cout << "Stop " << name << ": buses";
		for (const string& i : buses_on_stop)
		{
			cout << " " << i;
		}
		cout << endl;
	}
}

}