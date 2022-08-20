#include "transport_catalogue.h"

using namespace std;

namespace transport_catalogue
{

	void TransportCatalogue::AddStop(string name, geo::Coordinates coor)
	{
		stops.push_back({ name, coor });
		stopname_to_stop.insert({ stops.back().name_stop, &(stops.back()) });
	}

	Stop* TransportCatalogue::FindStop(string_view name) const
	{
		auto iter = stopname_to_stop.find(name);
		if (iter == stopname_to_stop.end())
			return nullptr;

		return iter->second;
	}

	void TransportCatalogue::AddBus(string_view bus_name, vector<string>& bus, bool is_roundtrip)
	{
		vector<Stop*> stops(bus.size());

		transform(
			bus.begin(), bus.end(),
			stops.begin(),
			[this, bus_name](string_view stop)
			{
				return stopname_to_stop[stop];
			}
		);
		buses.push_back({ string(bus_name), stops, is_roundtrip });
		busname_to_bus.insert({ buses.back().name_bus , &(buses.back()) });

		Stop* stop1 = buses.back().bus[0];

		for_each(
			stops.begin(), stops.end(),
			[this, &stop1](Stop* stop2)
			{
				all_buses_on_stops[stop2->name_stop].insert(&(buses.back()));

				if (geo_stops_distance.count({ stop1, stop2 }) == 0)
					geo_stops_distance.insert({ { stop1, stop2}, ComputeDistance(stop1->coor, stop2->coor) });

				stop1 = stop2;
			}
		);
	}

	Bus* TransportCatalogue::FindBus(std::string_view name) const
	{
		auto iter = busname_to_bus.find(name);
		if (iter == busname_to_bus.end())
			return nullptr;
		return iter->second;
	}

	BusInfo TransportCatalogue::GetBusInfo(string_view name) const
	{
		Bus* iter = FindBus(name);
		if (iter == nullptr)
			return {};

		vector<Stop*>& bus = iter->bus;
		unordered_set<Stop*> unique_bus(bus.begin(), bus.end());

		double geo_distance = 0;
		double earth_distance = 0;
		Stop* stop1 = bus[0];
		for (size_t i = 1; i < bus.size(); i++)
		{
			Stop* stop2 = bus[i];

			geo_distance += GetGeoDistance({ stop1, stop2 });
			earth_distance += GetEarthDistance({ stop1, stop2 });

			stop1 = stop2;
		}
		return { unique_bus.size(), earth_distance,  earth_distance / geo_distance };
	}

	unordered_set<Bus*> TransportCatalogue::GetAllBusOnStop(string_view name) const
	{
		return all_buses_on_stops.at(name);
	}

	void TransportCatalogue::SetEarthDistance(pair <string, string> stop_and_next_, double distance)
	{
		pair <Stop*, Stop*> stop_and_next_stop = { stopname_to_stop[stop_and_next_.first], stopname_to_stop[stop_and_next_.second] };
		earth_stops_distance[stop_and_next_stop] = distance;
	}
	double TransportCatalogue::GetEarthDistance(pair <Stop*, Stop*> stop_and_next_stop) const
	{
		auto iter = earth_stops_distance.find(stop_and_next_stop);
		if (iter != earth_stops_distance.end())
			return iter->second;
		else
			return earth_stops_distance.at({ stop_and_next_stop.second, stop_and_next_stop.first });
	}

	double TransportCatalogue::GetGeoDistance(pair <Stop*, Stop*> stop_and_next_stop) const
	{
		auto iter = geo_stops_distance.find(stop_and_next_stop);
		if (iter != geo_stops_distance.end())
			return iter->second;
		else
			return geo_stops_distance.at({ stop_and_next_stop.second, stop_and_next_stop.first });
	}
}