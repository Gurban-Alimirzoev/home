#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <deque>
#include <string_view>
#include <set>
#include <functional>
#include <algorithm>
#include "geo.h"

struct Stop
{
	std::string name_stop;
	Coordinates coor;
};

struct Bus
{
	std::string name_bus;
	std::vector<Stop*> bus;
};

class PairStopStopHasher {
public: 
	size_t operator()(const std::pair< Stop*, Stop*> pair_stops) const
	{
		/*size_t first_hash_lat = coor_hasher(pair_stops.first->coor.lat);
		size_t first_hash_lng = coor_hasher(pair_stops.first->coor.lng);
		size_t second_hash_lat = coor_hasher(pair_stops.second->coor.lat);
		size_t second_hash_lng = coor_hasher(pair_stops.second->coor.lng);

		return first_hash_lat * 37 + first_hash_lng * 37 * 37 + second_hash_lat * 37 * 37 + second_hash_lng * 37 * 37;*/
		return coor_hasher(pair_stops.first) + coor_hasher(pair_stops.second);
	}

	//std::hash<double> coor_hasher;
	std::hash<const void*> coor_hasher;
};

class TransportCatalogue {

public:
	void AddStop(std::string name, Coordinates coor);
	Stop* FindStop(std::string name);
	void AddBus(std::string name, std::deque<std::string> bus);
	Bus FindBus(std::string name);
	std::pair<int, double> GetBusInfo(std::string name);

private:
	std::deque <Stop> stops;
	//std::unordered_map <std::string_view, Stop> stopname_to_stop;
	std::unordered_map <std::string_view, Stop*> stopname_to_stop;
	std::deque <Bus> buses;
	std::unordered_map <std::string_view, Bus*> busname_to_bus;
	std::unordered_map <std::pair < Stop*, Stop*>, double, PairStopStopHasher> stops_distance;
};