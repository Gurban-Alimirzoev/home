#pragma once

#include <string>
#include <vector>
#include "geo.h"

namespace transport_catalogue
{

	struct Stop
	{
		std::string name_stop;
		geo::Coordinates coor;
	};

	struct Bus
	{
		std::string name_bus;
		std::vector<Stop*> bus;		
		bool is_roundtrip;
	};

	struct BusInfo
	{
		size_t unique_stop;
		double route_length;
		double curvature;
	};

}

namespace transport_catalogue::detail
{

	class PairStopStopHasher {
	public:
		size_t operator()(const std::pair< Stop*, Stop*> pair_stops) const
		{
			return coor_hasher(pair_stops.first) + coor_hasher(pair_stops.second) * 11;
		}
		std::hash<const void*> coor_hasher;
	};

}