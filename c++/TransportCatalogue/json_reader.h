#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "json.h"
#include "transport_catalogue.h"
#include "request_handler.h"

/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */

class JsonReader
{
public:
	JsonReader() = default;

	JsonReader(std::istream& input, std::ostream& out)
		: input(input), out(out), input_json(json::Load(input)), handler(db)
	{}

	void Requests();

	void BaseRequests(json::Array base_requests);
	void BaseRequest_AddBus();
	void BaseRequests_AddStop(json::Dict stop);

	void StatRequests(json::Array stat_requests);
	void StatRequests_PrintBusRequest(json::Dict bus_request);
	void StatRequests_PrintStopRequest(json::Dict stop_request);

private:
	std::istream& input;
	std::ostream& out;
	json::Document input_json;
	//json::Document output_json;
	transport_catalogue::TransportCatalogue db;
	transport_catalogue::RequestHandler handler;
	std::deque <json::Dict> buses;
	json::Array answer;
};