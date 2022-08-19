#pragma once
#include "json.h"
#include "transport_catalogue.h"

/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */

class JsonReader
{
public:

	JsonReader(std::istream& input)
		: input_json(json::Load(input))
	{}

	void BaseRequests();
	void StatRequests();

private:
	json::Document input_json;
	transport_catalogue::TransportCatalogue db;
};