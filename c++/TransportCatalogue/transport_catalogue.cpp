#include "transport_catalogue.h"

using namespace std;

void TransportCatalogue::AddStop(string name, Coordinates coor)
{

	stops.push_back({ name, coor });
	stopname_to_stop.insert({ stops.back().name_stop, &(stops.back()) });

	size_t count = 0;
	Stop stop2 = stops[count];
	while (stop2.name_stop != stops.back().name_stop)
	{
		stops_distance.insert({ { &stop2 , &stops.back()}, ComputeDistance(stop2.coor, stops.back().coor) });
		stops_distance.insert({ { &stops.back(), &stop2 }, ComputeDistance(stops.back().coor, stop2.coor) });
		stop2 = stops[count++];
	}
}

const Stop TransportCatalogue::FindStop(string name)
{
	return *stopname_to_stop.at(name);
}

void TransportCatalogue::AddBus(string name, vector<string> bus)
{
	vector<Stop*> bus_ptr;
	for (string i : bus)
	{
		bus_ptr.push_back(stopname_to_stop[i]);
	}
	buses.push_back({ name, bus_ptr });
	busname_to_bus.insert({ buses.back().name_bus ,& (buses.back()) });
}

const Bus TransportCatalogue::FindBus(string name)
{
	return *busname_to_bus.at(name);
}

pair<int, double_t> TransportCatalogue::GetBusInfo(string name)
{
	if (busname_to_bus.find(name) == busname_to_bus.end())
	{
		return {};
	}
	vector<Stop*> bus = busname_to_bus.at(name)->bus;
	set<Stop*> unique_bus(bus.begin(), bus.end());

	double_t bus_distance = 0;
	bool flag = true;
	Stop* first_stop = bus[0];
	for (Stop* i : bus)
	{
		if (flag)
		{
			//size_t hash = PairStopStopHasher({ bus[0], bus[1] });
			bus_distance += stops_distance[{ bus[0], bus[1] }];
			flag = false;
		}
		else
		{
			bus_distance += stops_distance[{first_stop, i}];
			first_stop = i;
		}
	}
	return { unique_bus.size(), bus_distance };
}
