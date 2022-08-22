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
		: input(input), out(out), handler(db)
	{}

	void ReadJson();
	void Requests();
	json::Document GetAnswerJson() const;
	void PrintAnswerJson();

private:
	std::istream& input;
	std::ostream& out;
	transport_catalogue::TransportCatalogue db;
	json::Document input_json;
	transport_catalogue::RequestHandler handler;
	std::deque <json::Dict> buses;
	json::Array answer;

	void BaseRequests(json::Array base_requests);
	void BaseRequest_AddBus();
	void BaseRequests_AddStop(json::Dict stop);

	void StatRequests(json::Array stat_requests);
	void StatRequests_PrintBusRequest(json::Dict bus_request);
	void StatRequests_PrintStopRequest(json::Dict stop_request);
};