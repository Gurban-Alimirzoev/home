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

namespace transport_catalogue
{

struct Stop
{
	std::string name_stop;
	transport_catalogue::geo::Coordinates coor;
};

struct Bus
{
	std::string name_bus;
	std::vector<Stop*> bus;
};

struct BusInfo
{
	size_t unique_stop;
	double route_length;
	double curvature;
};

class PairStopStopHasher {
public:
	size_t operator()(const std::pair< Stop*, Stop*> pair_stops) const
	{
		return coor_hasher(pair_stops.first) + coor_hasher(pair_stops.second) * 11;
	}
	std::hash<const void*> coor_hasher;
};

class PairStringStringHasher {
public:
	size_t operator()(const std::pair<std::string, std::string> pair_stops) const
	{
		return coor_hasher(pair_stops.first) + coor_hasher(pair_stops.second) * 11;
	}
	std::hash<std::string> coor_hasher;
};

class TransportCatalogue {

public:
	void AddStop(std::string name, transport_catalogue::geo::Coordinates coor, std::vector<std::pair<std::string, std::string>> associated_stops);
	Stop* FindStop(std::string name);
	void AddBus(std::string name, std::vector<std::string> bus);
	Bus* FindBus(std::string name);
	BusInfo GetBusInfo(std::string name);
	std::set<std::string> GetAllBusOnStop(std::string name);
	void SetEarthDistance(std::pair <std::string, std::string> stop_and_next_stop, double distance);
	double GetEarthDistance(std::pair <std::string, std::string> stop_and_next_stop);

private:
	std::deque <Stop> stops;
	std::unordered_map <std::string_view, Stop*> stopname_to_stop;
	std::deque <Bus> buses;
	std::unordered_map <std::string_view, Bus*> busname_to_bus;
	std::unordered_map <std::string, std::unordered_set<std::string>> all_buses_on_stops;
	std::unordered_map <std::pair < Stop*, Stop*>, double, PairStopStopHasher> geo_stops_distance;
	std::unordered_map <std::pair <std::string, std::string>, double, PairStringStringHasher> earth_stops_distance;
};

}