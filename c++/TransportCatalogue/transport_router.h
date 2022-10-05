#pragma once
#include <algorithm>
#include <unordered_map>
#include <string>

#include "graph.h"
#include "router.h"
#include "transport_catalogue.h"

namespace transport_catalogue::route
{
	struct RouteInfo {
		double weight;
		std::vector<size_t> stops_pair_id;
	};

	class TransportRouter
	{
	public:
		TransportRouter(const TransportCatalogue& db_)
			: db(db_)
		{}

		void DoAfterAddingAllStops();
		void DoAfterAddingAllBuses();

		void SetRoutingSettings(int whait_time, double speed);
		void SetBus(std::string bus_name, std::vector<std::string>& stops);
		void SetStop(std::string stop);

		std::optional<RouteInfo> BuildRoute(const std::string& start, const std::string& finish);

		std::tuple<transport_catalogue::route::BusRoute*, double> GetRouteAndDistance(size_t edge);
		double GetDistance(const RouteInfo& start_to_finish) const;
		size_t GetStopId(const std::string& stop);

	private:
		std::optional<graph::Router<double>> router;
		const TransportCatalogue& db;
		transport_catalogue::route::Settings settings;
		std::unordered_map <std::string, size_t> stop_name_and_vertex_id;
		std::unordered_map < size_t, transport_catalogue::route::BusRoute> vertex_id_and_bus_name_;
		graph::DirectedWeightedGraph<double> graph_;
	};

}