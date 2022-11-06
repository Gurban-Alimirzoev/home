#include "transport_router.h"

using namespace std;
using namespace transport_catalogue;
using namespace graph;

namespace transport_catalogue::route
{

	void TransportRouter::DoAfterAddingAllStops()
	{
		graph_ = DirectedWeightedGraph<double>(db.GetAllStops().size());
	}

	void TransportRouter::SetRoutingSettings(int whait_time, double speed)
	{
		settings.bus_wait_time = whait_time;
		settings.bus_velocity = speed;
	}

	void TransportRouter::SetBus(string bus_name, vector<string>& stops)
	{
		int first_s = 0;
		int second_s = 0;
		string second_stop, prev_stop;
		for (auto stop : stops)
		{
			first_s++;
			double run_time = 0;
			int span_count = 0;
			for (second_s = first_s; second_s < stops.size(); second_s++)
			{
				second_stop = stops[second_s];
				prev_stop = stops[second_s - 1];
				double distance = db.GetEarthDistance({ db.FindStop(prev_stop), db.FindStop(second_stop) });
				run_time += (distance / (settings.bus_velocity * m_in_km));
				size_t id = graph_.AddEdge({
					stop_name_and_vertex_id[stop],
					stop_name_and_vertex_id[second_stop],
					run_time + settings.bus_wait_time
					});
				vertex_id_and_bus_name_[id] = { bus_name , stop, second_stop, ++span_count };
			}
		}
	}

	void TransportRouter::SetStop(std::string stop)
	{
		if (stop_name_and_vertex_id.find(stop) == stop_name_and_vertex_id.end())
			stop_name_and_vertex_id[stop] = stop_name_and_vertex_id.size();
	}

	optional<RouteInfo> TransportRouter::BuildRoute(const string& start, const string& finish)
	{

		auto start_to_finish = router->BuildRoute(
			{ GetStopId(start) },
			{ GetStopId(finish) });
		if (start_to_finish == nullopt)
		{
			return nullopt;
		}
		return RouteInfo{ start_to_finish->weight, start_to_finish->edges };
	}

	tuple<BusRoute*, double> TransportRouter::GetRouteAndDistance(size_t edge)
	{
		double weight = graph_.GetEdge(edge).weight - settings.bus_wait_time;
		return { &(vertex_id_and_bus_name_[edge]), weight };
	}

	double TransportRouter::GetDistance(const RouteInfo& start_to_finish) const
	{
		return start_to_finish.weight;
	}

	size_t TransportRouter::GetStopId(const string& stop)
	{
		return stop_name_and_vertex_id.at(stop);
	}

	void TransportRouter::DoAfterAddingAllBuses()
	{
		router.emplace(graph_);
	}

	const Settings TransportRouter::GetSettings() const
	{
		return settings;
	}
}