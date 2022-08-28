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

class JsonReader
{
public:
	JsonReader() = default;
	JsonReader(std::istream& input, std::ostream& out)
		: input(input)
		, out(out)
		, handler(db, renderer)
		, input_json(json::Load(input))
	{}

	void ReadJson();
	void Requests();
	json::Document GetAnswerJson() const;
	void PrintAnswerJson();

private:
	std::istream& input;
	std::ostream& out;
	transport_catalogue::TransportCatalogue db;
	transport_catalogue::RequestHandler handler;
	renderer::MapRenderer renderer;
	std::unordered_map < std::string, std::vector<std::pair<std::string, double>>> buffer_distance;
	std::deque <json::Dict> buses;
	json::Document input_json;
	json::Array answer;

	void BaseRequests(json::Array base_requests);
	void BaseRequest_AddBus();
	void BaseRequests_AddStop(json::Dict stop);

	void SetDistance();
	void Transform(const json::Array& stops_input, std::vector<std::string>& stops_output);

	void StatRequests(json::Array stat_requests);
	void StatRequests_PrintBusRequest(json::Dict bus_request);
	void StatRequests_PrintStopRequest(json::Dict stop_request);

	void RenderRequests(json::Dict render_requests);
};