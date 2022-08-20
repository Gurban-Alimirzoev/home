#pragma once
#include <vector>
#include <string>
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

	JsonReader(std::istream& input)
		: input(input), input_json(json::Load(input))
	{}

	void Requests();
	void BaseRequests(json::Array base_requests);
	void StatRequests(json::Array stat_requests);

private:
	std::istream& input;
	json::Document input_json;
	json::Document output_json;
	transport_catalogue::TransportCatalogue db;
};