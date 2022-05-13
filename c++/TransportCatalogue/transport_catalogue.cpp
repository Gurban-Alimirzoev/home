#include "transport_catalogue.h"

using namespace std;

namespace transport_catalogue
{

void TransportCatalogue::AddStop(string name, geo::Coordinates coor, vector<pair<string, string>> associated_stops)
{
	stops.push_back({ name, coor });
	stopname_to_stop.insert({ stops.back().name_stop, &(stops.back()) });
	for (pair<string, string> i : associated_stops)
	{
		SetEarthDistance({ name, move(i.first) }, atof(i.second.c_str()));
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

void TransportCatalogue::AddBus(string name, vector<string> bus)
{
	vector<Stop*> bus_ptr(bus.size());

	transform(
		//execution::par,
		bus.begin(), bus.end(),
		bus_ptr.begin(),
		[this, name](string_view i)
		{
			return stopname_to_stop[i];
		}
	);
	buses.push_back({ name, bus_ptr });
	busname_to_bus.insert({ buses.back().name_bus ,&(buses.back()) });

	for_each(
		//execution::par,
		bus.begin(), bus.end(),
		[this, name](string i)
		{
			all_buses_on_stops[i].insert(name);
		}
	);

}

Bus* TransportCatalogue::FindBus(string name)
{
	if (busname_to_bus.find(name) == busname_to_bus.end())
	{
		static Bus b;
		return &b;
	}
	return busname_to_bus.at(name);
}

BusInfo TransportCatalogue::GetBusInfo(string name)
{
	if (busname_to_bus.find(name) == busname_to_bus.end())
	{
		return {};
	}
	vector<Stop*> &bus = busname_to_bus.at(name)->bus;
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
			geo_distance += geo_stops_distance[{ stop1, stop2 }];
		}

		earth_distance += GetEarthDistance({ stop1->name_stop, stop2->name_stop });

		stop1 = stop2;
	}
	return { unique_bus.size(), earth_distance,  earth_distance / geo_distance };
}

set<string> TransportCatalogue::GetAllBusOnStop(string name)
{
	set<string> var(all_buses_on_stops[name].begin(), all_buses_on_stops[name].end());
	return var;
}

void TransportCatalogue::SetEarthDistance(std::pair <string, string> stop_and_next_stop, double distance)
{
	earth_stops_distance[stop_and_next_stop] = distance;
}
double TransportCatalogue::GetEarthDistance(std::pair <string, string> stop_and_next_stop)
{
	if (earth_stops_distance.find(stop_and_next_stop) != earth_stops_distance.end())
	{
		return earth_stops_distance.at(stop_and_next_stop);
	}
	else
	{
		return earth_stops_distance[{stop_and_next_stop.second, stop_and_next_stop.first}];
	}
}

}