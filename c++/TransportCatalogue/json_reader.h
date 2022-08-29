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
	JsonReader(std::istream& input, std::ostream& out)
		: input(input)
		, out(out)
		, input_json(json::Load(input))
	{}

	void ReadJson();
	transport_catalogue::TransportCatalogue GetDB() const;
	void ParseRequests();
	void BaseRequests();
	void StatRequests();
	void ParseRenderRequests();

	json::Document GetAnswerToStatRequests() const;
	void PrintAnswerToStatRequests();

	renderer::Settings GetSettings() const;
	std::vector<transport_catalogue::Stop*> GetAllStopOnBus();

private:
	std::istream& input;
	std::ostream& out;
	transport_catalogue::TransportCatalogue db;
	json::Array base_requests;
	json::Array stat_requests;
	json::Dict render_requests;

	std::unordered_map < std::string, std::vector<std::pair<std::string, double>>> buffer_distance;

	std::deque <json::Dict> buses;
	json::Document input_json;
	json::Array answer;
	renderer::Settings settings;
	void BaseRequest_AddBus();
	void BaseRequests_AddStop(json::Dict stop);

	void SetDistance();
	void Transform(const json::Array& stops_input, std::vector<std::string>& stops_output);

	void StatRequests_PrintBusRequest(json::Dict bus_request);
	void StatRequests_PrintStopRequest(json::Dict stop_request);

	void MakeSettings(json::Dict render_requests);
	svg::Color RenderRequests_RgbOrRgba(json::Array color);
};