#pragma once
#include <algorithm>
#include <unordered_map>
#include <string>

#include "graph.h"
#include "router.h"
#include "transport_catalogue.h"

namespace transport_catalogue::route
{

	class TransportRouter
	{
	public:
		TransportRouter(const TransportCatalogue& db_)
			: db(db_)
		{}

		void SetRouterSize(size_t size_r);
		void SetRoutingSettings(int whait_time, double speed);
		void SetBus(std::string bus_name, std::vector<std::string>& stops);
		void SetStop(std::string stop);

		graph::Router<double> MakeRouter();

		std::unordered_set<std::pair<transport_catalogue::route::BusRoute*, double>, transport_catalogue::route::PairBusRouteHasher> GetRouteAndDistance(graph::Router<double>::RouteInfo start_to_finish);
		double GetDistance(const graph::Router<double>::RouteInfo& start_to_finish) const;
		size_t GetStopId(std::string stop);
		transport_catalogue::route::Settings GetSettings();

	private:
		const TransportCatalogue& db;
		transport_catalogue::route::Settings settings;
		std::unordered_map <std::string, size_t> stop_name_and_vertex_id;
		std::unordered_map < size_t, transport_catalogue::route::BusRoute> vertex_id_and_bus_name_;
		graph::DirectedWeightedGraph<double> graph_;
		//std::unordered_map<transport_catalogue::route::BusRoute, double> route_and_distance;
	};

}