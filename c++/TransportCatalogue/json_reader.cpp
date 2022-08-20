#include "json_reader.h"

/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */

void JsonReader::Requests()
{
	json::Node requests = input_json.GetRoot();

	for (auto request : requests.AsMap())
	{
		if (request.first == "base_requests")
			BaseRequests(request.second.AsArray());
		else
			StatRequests(request.second.AsArray());
	}
}


void JsonReader::BaseRequests(json::Array base_requests)
{
	for (json::Node base_request : base_requests)
	{
		json::Dict bus_or_stop = base_request.AsMap();
		size_t size = bus_or_stop.at("stops").AsArray().size();
		std::vector<std::string> stops(size);
		for (json::Node s : bus_or_stop.at("stops").AsArray())
		{
			stops.push_back(s.AsString());
		}

		if (bus_or_stop.at("type") == "Bus")
		{
			db.AddBus(
				bus_or_stop.at("name").AsString(),
				stops,
				bus_or_stop.at("is_roundtrip").AsBool()
			);
		}
		else
		{
			db.AddStop(
				bus_or_stop.at("name").AsString(),
				{
					bus_or_stop.at("latitude").AsDouble(),
					bus_or_stop.at("longitude").AsDouble()
				}
			);
			json::Dict road_distances = bus_or_stop.at("road_distances").AsMap();
			for (auto dist : road_distances)
			{
				db.SetEarthDistance(
					{
						bus_or_stop.at("name").AsString(),
						dist.first
					},
					dist.second.AsDouble()
				);
			}
		}
	}
}

void StatRequests(json::Array stat_requests)
{
	for (json::Node stat_request : stat_requests)
	{
		json::Dict bus_or_stop = stat_request.AsMap();
		if (bus_or_stop.at("type") == "Stop")

	}
}