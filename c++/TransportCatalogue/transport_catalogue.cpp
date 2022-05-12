#include "transport_catalogue.h"

using namespace std;

void TransportCatalogue::AddStop(string name, Coordinates coor)
{

	stops.push_back({ name, coor });
	stopname_to_stop.insert({ stops.back().name_stop, &(stops.back()) });

	Stop &stop1 = stops.back();

	for (size_t i = 0; i < stops.size() - 1; i++)
	{
		stops_distance.insert({ { &(stops[i]) , &stop1}, ComputeDistance((stops[i]).coor, stop1.coor)});
		stops_distance.insert({ { &stop1, &(stops[i]) }, ComputeDistance(stop1.coor, (stops[i]).coor) });
	}
}

Stop* TransportCatalogue::FindStop(string name)
{
	if (stopname_to_stop.find(name) == stopname_to_stop.end())
	{
		static Stop s;
		return &s;
	}
	return stopname_to_stop.at(name);
}

void TransportCatalogue::AddBus(string name, deque<string> bus)
{
	vector<Stop*> bus_ptr(bus.size());

	transform(
		bus.begin(), bus.end(),
		bus_ptr.begin(),
		[this](string i)
		{
			return stopname_to_stop[i];
		}
	);
	buses.push_back({ name, bus_ptr });
	busname_to_bus.insert({ buses.back().name_bus ,& (buses.back()) });
}

Bus TransportCatalogue::FindBus(string name)
{
	if (busname_to_bus.find(name) == busname_to_bus.end())
	{
		static Bus b;
		return b;
	}
	return *busname_to_bus.at(name);
}

pair<int, double_t> TransportCatalogue::GetBusInfo(string name)
{
	auto iter = busname_to_bus.find(name);
	if (iter == busname_to_bus.end())
	{
		return {};
	}
	vector<Stop*> bus = busname_to_bus.at(name)->bus;
	set<Stop*> unique_bus(bus.begin(), bus.end());

	double_t bus_distance = 0;
	Stop* first_stop = bus[0];
	for (size_t i = 1; i < bus.size(); i++)
	{
		bus_distance += stops_distance[{first_stop, bus[i]}];
		first_stop = bus[i];
	}
	return { unique_bus.size(), bus_distance };
}
