#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <string_view>
#include <set>
#include <functional>
#include <execution>
#include <algorithm>
#include <future>
#include <iterator>
#include "geo.h"
#include "domain.h"

namespace transport_catalogue
{

	class TransportCatalogue {

	public:
		void AddStop(std::string name, geo::Coordinates coor);
		void AddBus(std::string_view name, std::vector<std::string>& stops_on_bus, bool is_roundtrip);

		Stop* FindStop(std::string_view name) const;
		Bus* FindBus(std::string_view name) const;

		BusInfo GetBusInfo(std::string_view name) const;
		std::unordered_set<Bus*> GetAllBusOnStop(std::string_view name) const;

		void SetEarthDistance(std::pair <std::string, std::string> stop_and_next_stop, double distance);
		double GetEarthDistance(std::pair < Stop*, Stop* > stop_and_next_stop) const;
		double GetGeoDistance(std::pair <Stop*, Stop*> stop_and_next_stop) const;

	private:
		std::deque <Stop> stops;
		std::unordered_map <std::string_view, Stop*> stopname_to_stop;
		std::deque <Bus> buses;
		std::unordered_map <std::string_view, Bus*> busname_to_bus;
		std::unordered_map <std::string_view, std::unordered_set<Bus*>> all_buses_on_stops;
		std::unordered_map <std::pair < Stop*, Stop*>, double, detail::PairStopStopHasher> geo_stops_distance;
		std::unordered_map <std::pair <Stop*, Stop*>, double, detail::PairStopStopHasher> earth_stops_distance;
	
	};

}