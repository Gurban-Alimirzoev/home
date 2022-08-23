#include "json_reader.h"

using namespace std;
using namespace transport_catalogue;
using namespace json;

void JsonReader::ReadJson()
{
	input_json = json::Load(input);
}

void JsonReader::Requests()
{
	Node requests = input_json.GetRoot();
	for (auto request : requests.AsMap())
	{
		if (request.first == "base_requests")
			BaseRequests(request.second.AsArray());
		else
			StatRequests(request.second.AsArray());
	}
}

void JsonReader::BaseRequests(Array base_requests)
{
	for (Node base_request : base_requests)
	{
			Dict bus_or_stop = base_request.AsMap();
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
			stops
		);
	}
}

void JsonReader::Transform(const json::Array& stops_input, vector<string>& stops_output)
{
	transform(
		stops_input.begin(),
		stops_input.end(),
		stops_output.begin(),
		[](Node stop)
		{return stop.AsString(); }
	);
}

void JsonReader::BaseRequests_AddStop(Dict stop)
{

	db.AddStop(
		stop.at("name").AsString(),
		{
			stop.at("latitude").AsDouble(),
			stop.at("longitude").AsDouble()
		}
	);
	Dict road_distances = stop.at("road_distances").AsMap();
	for (auto [name_second_stop, distance] : road_distances)
	{
		buffer_distance[
			stop.at("name").AsString()].push_back({
				name_second_stop, distance.AsDouble()});
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

void JsonReader::StatRequests(Array stat_requests)
{
	for (Node stat_request : stat_requests)
	{
		Dict bus_or_stop = stat_request.AsMap();
		if (bus_or_stop.at("type").AsString() == "Stop")
			StatRequests_PrintStopRequest(bus_or_stop);
		else
			StatRequests_PrintBusRequest(bus_or_stop);
	}
}

void JsonReader::StatRequests_PrintStopRequest(Dict stop_request)
{
	if (!handler.ChekStop(stop_request.at("name").AsString()))
	{
		answer.emplace_back(Dict{
			{"error_message",
			Node(string("not found"))},
			{"request_id",
			stop_request.at("id")},
			});
	}
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
			}
		);
		sort(buses_on_stop_str.begin(), buses_on_stop_str.end());
		vector<Node> buses_on_stop_vector_bus(buses_on_stop.size());
		transform(
			buses_on_stop_str.begin(),
			buses_on_stop_str.end(),
			buses_on_stop_vector_bus.begin(),
			[](string bus)
			{
				return Node(bus);
			}
		);
		answer.emplace_back(Dict{
			{"buses",
			Node(buses_on_stop_vector_bus)},
			{"request_id", stop_request.at("id")}
			});
	}
}

void JsonReader::StatRequests_PrintBusRequest(Dict bus_request)
{
	if (!handler.ChekBus(bus_request.at("name").AsString()))
	{
		answer.emplace_back(Dict{
			{"error_message",
			Node(string("not found"))},
			{"request_id",
			bus_request.at("id")},
			});
	}
	else
	{
		string name = bus_request.at("name").AsString();
		BusInfo info = *(handler.GetBusStat(name));
		int stop_count = handler.GetStopCount(name);
		answer.emplace_back(Dict{
			{"curvature",
			Node(info.curvature)},
			{"request_id",
			bus_request.at("id")},
			{"route_length",
			Node(info.route_length)},
			{"stop_count",
			Node(stop_count)},
			{"unique_stop_count",
			Node(static_cast<int>(info.unique_stop))},
			});
	}
}

json::Document JsonReader::GetAnswerJson() const
{
	return Document(answer);
}

void JsonReader::PrintAnswerJson()
{
	Print(Document{ answer }, out);
}