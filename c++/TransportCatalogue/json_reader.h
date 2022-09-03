#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <utility>

#include "json.h"
#include "transport_catalogue.h"
#include "request_handler.h"
#include "map_renderer.h"

class JsonReader
{
public:
	JsonReader() = default;
	JsonReader(std::istream &input, std::ostream &out, renderer::MapRenderer &rendrer_)
		: input(input), out(out), input_json(json::Load(input)), rendrer(rendrer_), handler(db, rendrer_)
	{
	}

	void ParseRequests();
	void BaseRequests();
	void ParseRenderRequests();
	void StatRequests();
	json::Document GetAnswerToStatRequests() const;
	void PrintAnswerToStatRequests();

	renderer::Settings GetSettings() const;
	void AddRendererElements();

private:
	std::istream &input;
	std::ostream &out;
	json::Document input_json;
	json::Array base_requests;
	json::Array stat_requests;
	json::Array answer;
	json::Dict render_requests;
	renderer::MapRenderer &rendrer;
	renderer::Settings settings;
	transport_catalogue::TransportCatalogue db;
	transport_catalogue::RequestHandler handler;
	std::deque<json::Dict> buses;
	std::vector<std::pair<std::string, bool>> buses_sort;
	std::vector<transport_catalogue::Stop> var;
	std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> buffer_distance;

	void BaseRequest_AddBus();
	void BaseRequests_AddStop(json::Dict stop);

	void SetDistance();
	void Transform(const json::Array &stops_input, std::vector<std::string> &stops_output);

	void StatRequests_PrintBusRequest(json::Dict bus_request);
	void StatRequests_PrintStopRequest(json::Dict stop_request);
	void StatRequests_PrintMapRequests(json::Dict map_request);

	void MakeSettings(json::Dict render_requests);
	svg::Color RenderRequests_RgbOrRgba(json::Array color);

	void MakeMap();
	void AddStopsToMap();
	void AddBusesToMap();
};