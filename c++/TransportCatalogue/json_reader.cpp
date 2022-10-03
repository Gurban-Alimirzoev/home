#include "json_reader.h"

using namespace std;
using namespace transport_catalogue;
using namespace json;
using namespace renderer;
using namespace graph;

void JsonReader::ParseRequests()
{
	Node requests = input_json.GetRoot();
	for (auto request : requests.AsDict())
	{
		if (request.first == "base_requests")
			base_requests = request.second.AsArray();
		else if (request.first == "stat_requests")
			stat_requests = request.second.AsArray();
		else if (request.first == "routing_settings")
			routing_settings = request.second.AsDict();
		else
			rendering_settings = request.second.AsDict();
	}
}

void JsonReader::BaseRequests()
{
	for (Node base_request : base_requests)
	{
		Dict bus_or_stop = base_request.AsDict();
		if (bus_or_stop.at("type").AsString() == "Stop")
			BaseRequests_AddStop(bus_or_stop);
		else
			buses.push_back(bus_or_stop);
	}
	SetDistance();
	BaseRequest_AddBus();
}

void JsonReader::BaseRequest_AddBus()
{
	DirectedWeightedGraph<double> graph_start(buffer_distance.size() * buses.size());
	graph_ = graph_start;
	size_t id;
		
	for (auto bus : buses)
	{
		if (bus.at("stops").AsArray().empty())
			continue;
		vector<string> stops(bus.at("stops").AsArray().size());
		Transform(bus.at("stops").AsArray(), stops);
		if (!bus.at("is_roundtrip").AsBool())
		{
			size_t input_stop_size = bus.at("stops").AsArray().size();
			stops.resize(input_stop_size * 2 - 1);
			reverse(stops.begin(), stops.end());
			Transform(bus.at("stops").AsArray(), stops);
		}
		string bus_name = bus.at("name").AsString();
		db.AddBus(
			bus_name,
			stops);

		string first_stop = stops[0];
		string second_stop = stops[1];
		string memory_stop = first_stop;
		int i = 1;
		while(i != stops.size())
		{
			double distance = buffer_distance[first_stop][second_stop];
			unordered_map<string, double>& second_stop_and_distance = buffer_distance[first_stop];

			id = graph_.AddEdge({
			stop_name_and_vertex_id[first_stop],
			stop_name_and_vertex_id[second_stop],
			distance / (route_settings.bus_velocity * route::m_in_km)
				});

			vertex_id_and_bus_name_[id] = pair{ bus_name, memory_stop };
			
			first_stop = second_stop;
			second_stop = stops[i++];
		}
	}
}

void JsonReader::Transform(const json::Array& stops_input, vector<string>& stops_output)
{
	transform(
		stops_input.begin(),
		stops_input.end(),
		stops_output.begin(),
		[](Node stop)
		{ return stop.AsString(); });
}

void JsonReader::BaseRequests_AddStop(Dict stop)
{
	db.AddStop(
		stop.at("name").AsString(),
		{ stop.at("latitude").AsDouble(),
		 stop.at("longitude").AsDouble() });
	Dict road_distances = stop.at("road_distances").AsDict();

	string name_first = stop.at("name").AsString();

	if (stop_name_and_vertex_id.find(name_first) == stop_name_and_vertex_id.end())
		stop_name_and_vertex_id[name_first] = stop_name_and_vertex_id.size();

	for (auto [name_second_stop, distance] : road_distances)
	{
		buffer_distance[stop.at("name").AsString()].insert({ name_second_stop, distance.AsDouble() });
		
		if (stop_name_and_vertex_id.find(name_second_stop) == stop_name_and_vertex_id.end())
			stop_name_and_vertex_id[name_second_stop] = stop_name_and_vertex_id.size();
	}
}

void JsonReader::SetDistance()
{
	for (auto [stop, second_stops_and_distance] : buffer_distance)
	{
		for (auto [second_stop, distance] : second_stops_and_distance)
		{
			db.SetEarthDistance({ stop, second_stop }, distance);
		}
	}
}

void JsonReader::StatRequests()
{
	Router router = MakeRouter();
	for (Node stat_request : stat_requests)
	{
		Dict requests_type = stat_request.AsDict();
		if (requests_type.at("type").AsString() == "Stop")
		{
			StatRequests_PrintStopRequest(requests_type);
		}
		else if (requests_type.at("type").AsString() == "Bus")
		{
			StatRequests_PrintBusRequest(requests_type);
		}
		else if (requests_type.at("type").AsString() == "Route")
		{
			StatRequests_PrintRouteRequest(requests_type, router);
		}
		else
		{
			StatRequests_PrintMapRequests(requests_type);
		}
	}

	result_answer = Builder{}.Value(answers).Build();
}

void JsonReader::StatRequests_PrintStopRequest(Dict stop_request)
{
	if (!handler.ChekStop(stop_request.at("name").AsString()))
		answers.push_back(Builder{}
			.StartDict()
			.Key("error_message")
			.Value(string("not found"))
			.Key("request_id")
			.Value(stop_request.at("id").AsInt())
			.EndDict()
			.Build()
			.AsDict());
	else
	{
		std::unordered_set<Bus*> buses_on_stop = handler.GetBusesByStop(stop_request.at("name").AsString());
		deque<string> buses_on_stop_str(buses_on_stop.size());

		transform(
			buses_on_stop.begin(),
			buses_on_stop.end(),
			buses_on_stop_str.begin(),
			[](Bus* bus)
			{
				return (*bus).name_bus;
			});
		sort(buses_on_stop_str.begin(), buses_on_stop_str.end());
		vector<Node> buses_on_stop_vector_bus(buses_on_stop.size());
		transform(
			buses_on_stop_str.begin(),
			buses_on_stop_str.end(),
			buses_on_stop_vector_bus.begin(),
			[](string bus)
			{
				return Node(bus);
			});
		answers.push_back(Builder{}
			.StartDict()
			.Key("buses")
			.Value(buses_on_stop_vector_bus)
			.Key("request_id")
			.Value(stop_request.at("id").AsInt())
			.EndDict()
			.Build()
			.AsDict());
	}
}

void JsonReader::StatRequests_PrintBusRequest(Dict bus_request)
{
	string name = bus_request.at("name").AsString();
	if (!handler.ChekBus(name))
	{
		answers.push_back(Builder{}
			.StartDict()
			.Key("error_message")
			.Value(string("not found"))
			.Key("request_id")
			.Value(bus_request.at("id").AsInt())
			.EndDict()
			.Build()
			.AsDict());
	}
	else
	{
		BusInfo info = *(handler.GetBusStat(name));
		int stop_count = handler.GetStopCount(name);
		answers.push_back(Builder{}
			.StartDict()
			.Key("curvature")
			.Value(info.curvature)
			.Key("request_id")
			.Value(bus_request.at("id").AsInt())
			.Key("route_length")
			.Value(info.route_length)
			.Key("stop_count")
			.Value(stop_count)
			.Key("unique_stop_count")
			.Value(static_cast<int>(info.unique_stop))
			.EndDict()
			.Build()
			.AsDict());
	}
}

void JsonReader::StatRequests_PrintRouteRequest(Dict route_request, Router<double>& router)
{
	auto start_to_finish = router.BuildRoute(
		{stop_name_and_vertex_id[route_request.at("from").AsString()] },
		{stop_name_and_vertex_id[route_request.at("to").AsString()]});

 	if (start_to_finish == nullopt)
	{
		answers.push_back(Builder{}
			.StartDict()
			.Key("request_id")
			.Value(route_request.at("id").AsInt())
			.Key("error_message")
			.Value(string("not found"))
			.EndDict()
			.Build()
			.AsDict());
	}
	else
	{
		vector<size_t>& edges = start_to_finish->edges;
		string bus_name = vertex_id_and_bus_name_[edges[0]].first;
		string stop_name = vertex_id_and_bus_name_[edges[0]].second;
		double weight = graph_.GetEdge(edges[0]).weight;
		int span_count = 0;
		double total_time = start_to_finish->weight;
		for (auto edge : edges)
		{
			if (bus_name == vertex_id_and_bus_name_[edge].first)
			{
				span_count++;
				stop_name = vertex_id_and_bus_name_[edge].second;
				double weight = graph_.GetEdge(edge).weight;
			}
			else
			{
				Node stop(Builder{}
					.StartDict()
					.Key("stop_name")
					.Value(string(stop_name))
					.Key("time")
					.Value(route_settings.bus_wait_time)
					.Key("type")
					.Value(string("Wait"))
					.EndDict()
					.Build()
				);
				total_time += route_settings.bus_wait_time;
				items.push_back(stop);
				Node bus(Builder{}
					.StartDict()
					.Key("bus")
					.Value(string(bus_name))
					.Key("span_count")
					.Value(span_count)
					.Key("time")
					.Value(weight)
					.Key("type")
					.Value(string("Bus"))
					.EndDict()
					.Build()
				);
				bus_name = vertex_id_and_bus_name_[edge].first;
				items.push_back(bus);
			}
		}
		answers.push_back(Builder{}
			.StartDict()
			.Key("items")
			.Value(items)
			.Key("request_id")
			.Value(route_request.at("id").AsInt())
			.Key("total_time")
			.Value(total_time)
			.EndDict()
			.Build()
			.AsDict());
	}
}

void JsonReader::StatRequests_PrintMapRequests(Dict map_request)
{
	ostringstream out;
	rendrer.RenderMap(out);
	answers.push_back(Builder{}
		.StartDict()
		.Key("map")
		.Value(out.str())
		.Key("request_id")
		.Value(map_request.at("id").AsInt())
		.EndDict()
		.Build()
		.AsDict());
}

Document JsonReader::GetAnswerToStatRequests() const
{
	return Document(result_answer);
}

void JsonReader::PrintAnswerToStatRequests()
{
	Print(Document{ result_answer }, out);
}

void JsonReader::ParseRenderSettings()
{
	MakeRenderSettings(rendering_settings);
}

void JsonReader::ParseRouteSettings()
{
	MakeRouteSettings(routing_settings);
}

void JsonReader::MakeRenderSettings(Dict rendering_settings)
{
	if (!rendering_settings.empty())
	{
		render_settings.width = rendering_settings.at("width").AsDouble();
		render_settings.height = rendering_settings.at("height").AsDouble();
		render_settings.stop_label_font_size = rendering_settings.at("stop_label_font_size").AsInt();
		render_settings.bus_label_font_size = rendering_settings.at("bus_label_font_size").AsInt();

		render_settings.padding = rendering_settings.at("padding").AsDouble();
		render_settings.line_width = rendering_settings.at("line_width").AsDouble();
		render_settings.stop_radius = rendering_settings.at("stop_radius").AsDouble();
		render_settings.underlayer_width = rendering_settings.at("underlayer_width").AsDouble();

		render_settings.bus_label_offset.clear();
		for (auto offset : rendering_settings.at("bus_label_offset").AsArray())
			render_settings.bus_label_offset.push_back(offset.AsDouble());
		render_settings.stop_label_offset.clear();
		for (auto offset : rendering_settings.at("stop_label_offset").AsArray())
			render_settings.stop_label_offset.push_back(offset.AsDouble());

		if (rendering_settings.at("underlayer_color").IsString())
			render_settings.underlayer_color = rendering_settings.at("underlayer_color").AsString();
		else
			render_settings.underlayer_color = RenderRequests_RgbOrRgba(
				rendering_settings.at("underlayer_color").AsArray());

		for (auto color : rendering_settings.at("color_palette").AsArray())
		{
			if (color.IsString())
				render_settings.color_palette.push_back(
					color.AsString());
			else
				render_settings.color_palette.push_back(
					RenderRequests_RgbOrRgba(
						color.AsArray()));
		}
	}
}

svg::Color JsonReader::RenderRequests_RgbOrRgba(json::Array color)
{
	if (color.size() == 3)
	{
		return svg::Rgb{
			static_cast<uint8_t>(color[0].AsInt()), static_cast<uint8_t>(color[1].AsInt()), static_cast<uint8_t>(color[2].AsInt()) };
	}
	else
	{
		return svg::Rgba{
			static_cast<uint8_t>(color[0].AsInt()), static_cast<uint8_t>(color[1].AsInt()), static_cast<uint8_t>(color[2].AsInt()), color[3].AsDouble() };
	}
}

Settings JsonReader::GetRenderSettings() const
{
	return render_settings;
}

void JsonReader::AddRendererElements()
{
	MakeMap();
	AddBusesToMap();
	AddStopsToMap();
}

void JsonReader::MakeMap()
{
	auto stops = db.GetAllStops();
	var.resize(stops.size());
	transform(
		stops.begin(),
		stops.end(),
		var.begin(),
		[this](Stop stop)
		{ return stop; });

	sort(var.begin(), var.end());

	for (Stop stop : var)
	{
		if (!handler.GetBusesByStop(stop.name_stop).empty())
			rendrer.SavePoints({ stop.coor.lat, stop.coor.lng });
	}
}

void JsonReader::AddBusesToMap()
{
	buses_sort.resize(buses.size());
	transform(
		buses.begin(),
		buses.end(),
		buses_sort.begin(),
		[this](Dict str)
		{
			return pair{ str.at("name").AsString(),
				str.at("is_roundtrip").AsBool() };
		});

	sort(buses_sort.begin(), buses_sort.end());

	rendrer.MakeSphereProjector();
	for (auto [name, is_roundtrip] : buses_sort)
	{
		if (handler.ChekBus(name) && !handler.GetStopsByBus(name).empty())
			rendrer.AddBusLine(handler.GetStopsByBus(name));
	}

	rendrer.RestartNumberOfColor();
	for (auto [name, is_roundtrip] : buses_sort)
	{
		if (handler.ChekBus(name) && !handler.GetStopsByBus(name).empty())
			rendrer.AddBusNameOnMap(db.FindBus(name), is_roundtrip);
	}
}

void JsonReader::AddStopsToMap()
{
	rendrer.AddCircleStops();
	for (Stop stop : var)
	{
		if (!handler.GetBusesByStop(stop.name_stop).empty())
			rendrer.AddStopName(stop);
	}
}

void JsonReader::MakeRouteSettings(json::Dict routing_settings)
{
	if (!routing_settings.empty())
	{
		route_settings.bus_wait_time = routing_settings.at("bus_wait_time").AsInt();
		route_settings.bus_velocity = routing_settings.at("bus_velocity").AsDouble();
	}
}

Router<double> JsonReader::MakeRouter()
{
	for (auto [stop, id] : stop_name_and_vertex_id)
	{
		vertex_id_and_stop_name_[id] = stop;
	}
	Router router(graph_);
	return router;
}

