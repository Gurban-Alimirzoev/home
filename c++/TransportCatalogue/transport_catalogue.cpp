#include "transport_catalogue.h"

using namespace std;

namespace transport_catalogue
{

void TransportCatalogue::AddStop(string name, geo::Coordinates coor, vector<pair<string, double>> &associated_stops)
{
	stops.push_back({ name, coor });
	stopname_to_stop.insert({ stops.back().name_stop, &(stops.back()) });
	for (pair<string, double> associated_stop : associated_stops)
	{
		SetEarthDistance({ name, associated_stop.first }, associated_stop.second);
	}
}

Stop* TransportCatalogue::FindStop(string_view name) const
{
	auto iter = stopname_to_stop.find(name);
	if (iter == stopname_to_stop.end())
	{
		return nullptr;
	}
	return iter->second;
}

void TransportCatalogue::AddBus(string bus_name, vector<string>& bus)
{
	vector<Stop*> bus_ptr(bus.size());

	transform(
		bus.begin(), bus.end(),
		bus_ptr.begin(),
		[this, bus_name](string_view stop)
		{
			return stopname_to_stop[stop];
		}
	);
	buses.push_back({ bus_name, bus_ptr });
	busname_to_bus.insert({ buses.back().name_bus ,&(buses.back()) });

	for_each(
		bus.begin(), bus.end(),
		[this, bus_name](string stop)
		{
			all_buses_on_stops[stop].insert(bus_name);
		}
	);

}

Bus* TransportCatalogue::FindBus(std::string_view name) const
{
	auto iter = busname_to_bus.find(name);
	if (iter == busname_to_bus.end())
	{
		return nullptr;
	}
	return iter->second;
}

BusInfo TransportCatalogue::GetBusInfo(const string& name)
{
	Bus* iter = FindBus(name);
	if (iter == nullptr)
	{
		return {};
	}
	vector<Stop*> &bus = iter->bus;
	unordered_set<Stop*> unique_bus(bus.begin(), bus.end());

	double geo_distance = 0;
	double earth_distance = 0;

	Stop* stop1 = bus[0];
	for (size_t i = 1; i < bus.size(); i++)
	{
		Stop* stop2 = bus[i];

		if (geo_stops_distance.count({ stop1, stop2 }) == 0)
		{
			double distance_stop_stop = ComputeDistance(stop1->coor, stop2->coor);
			geo_distance += distance_stop_stop;
			geo_stops_distance.insert({ { stop1, stop2}, distance_stop_stop });
		}
		else 
		{
			geo_distance += geo_stops_distance.at({ stop1, stop2 });
		}

		earth_distance += GetEarthDistance({ stop1->name_stop, stop2->name_stop });

		stop1 = stop2;
	}
	return { unique_bus.size(), earth_distance,  earth_distance / geo_distance };
}

set<string> TransportCatalogue::GetAllBusOnStop(string& name)
{
	set<string> var(all_buses_on_stops[name].begin(), all_buses_on_stops[name].end());
	return var;
}

void TransportCatalogue::SetEarthDistance(std::pair <string_view, string_view> stop_and_next_stop, double distance)
{
	earth_stops_distance[stop_and_next_stop] = distance;
}
double TransportCatalogue::GetEarthDistance(std::pair <string_view, string_view> stop_and_next_stop) 
{
	auto iter = earth_stops_distance.find(stop_and_next_stop);
	if (iter != earth_stops_distance.end())
	{
		return iter->second;
	}
	else
	{
		return earth_stops_distance[{stop_and_next_stop.second, stop_and_next_stop.first}];
	}
}

}