#include "json_reader.h"

using namespace std;
using namespace transport_catalogue;
using namespace json;
using namespace renderer;

void JsonReader::ParseRequests()
{
	Node requests = input_json.GetRoot();
	for (auto request : requests.AsDict())
	{
		if (request.first == "base_requests")
			base_requests = request.second.AsArray();
		else if (request.first == "stat_requests")
			stat_requests = request.second.AsArray();
		else
			render_requests = request.second.AsDict();
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
		db.AddBus(
			bus.at("name").AsString(),
			stops);
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
	for (auto [name_second_stop, distance] : road_distances)
	{
		buffer_distance[stop.at("name").AsString()].push_back({ name_second_stop, distance.AsDouble() });
	}
}

void JsonReader::SetDistance()
{
	for (auto [stop, second_stops_and_distance] : buffer_distance)
	{
		for (auto [second_stop, distance] : second_stops_and_distance)
			db.SetEarthDistance({ stop, second_stop }, distance);
	}
}

void JsonReader::StatRequests()
{
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

void JsonReader::ParseRenderRequests()
{
	MakeSettings(render_requests);
}

void JsonReader::MakeSettings(Dict render_requests)
{
	if (!render_requests.empty())
	{
		settings.width = render_requests.at("width").AsDouble();
		settings.height = render_requests.at("height").AsDouble();
		settings.stop_label_font_size = render_requests.at("stop_label_font_size").AsInt();
		settings.bus_label_font_size = render_requests.at("bus_label_font_size").AsInt();

		settings.padding = render_requests.at("padding").AsDouble();
		settings.line_width = render_requests.at("line_width").AsDouble();
		settings.stop_radius = render_requests.at("stop_radius").AsDouble();
		settings.underlayer_width = render_requests.at("underlayer_width").AsDouble();

		settings.bus_label_offset.clear();
		for (auto offset : render_requests.at("bus_label_offset").AsArray())
			settings.bus_label_offset.push_back(offset.AsDouble());
		settings.stop_label_offset.clear();
		for (auto offset : render_requests.at("stop_label_offset").AsArray())
			settings.stop_label_offset.push_back(offset.AsDouble());

		if (render_requests.at("underlayer_color").IsString())
			settings.underlayer_color = render_requests.at("underlayer_color").AsString();
		else
			settings.underlayer_color = RenderRequests_RgbOrRgba(
				render_requests.at("underlayer_color").AsArray());

		for (auto color : render_requests.at("color_palette").AsArray())
		{
			if (color.IsString())
				settings.color_palette.push_back(
					color.AsString());
			else
				settings.color_palette.push_back(
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

Settings JsonReader::GetSettings() const
{
	return settings;
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