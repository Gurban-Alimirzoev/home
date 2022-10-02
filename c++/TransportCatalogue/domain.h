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

		bool operator<(const Stop& rhs)
		{
			return name_stop < rhs.name_stop;
		}

		bool operator>(const Stop& rhs)
		{
			return name_stop > rhs.name_stop;
		}
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

namespace transport_catalogue::route
{
	struct Settings
	{
		double bus_wait_time = 1;
		double bus_velocity = 1;
	};

	inline double m_in_km = 16.666667;
}