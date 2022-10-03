#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <tuple>

#include "json_builder.h"
#include "transport_catalogue.h"
#include "request_handler.h"
#include "map_renderer.h"
#include "graph.h"
#include "route.h"

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
	void ParseRenderSettings();
	void ParseRouteSettings();
	void StatRequests();
	json::Document GetAnswerToStatRequests() const;
	void PrintAnswerToStatRequests();

	renderer::Settings GetRenderSettings() const;
	void AddRendererElements();

private:
	std::istream &input;
	std::ostream &out;
	json::Document input_json;
	json::Array base_requests;
	json::Array stat_requests;
	json::Array answers;
	json::Node result_answer;
	json::Dict routing_settings;
	json::Dict rendering_settings;
	renderer::MapRenderer &rendrer;
	renderer::Settings render_settings;
	transport_catalogue::route::Settings route_settings;
	transport_catalogue::TransportCatalogue db;
	transport_catalogue::RequestHandler handler;
	std::deque<json::Dict> buses;
	std::vector<std::pair<std::string, bool>> buses_sort;
	std::vector<transport_catalogue::Stop> var;
	std::unordered_map<std::string, std::unordered_map<std::string, double>> buffer_distance;
	std::unordered_map <std::string, size_t> stop_name_and_vertex_id;
	std::unordered_map < size_t, std::string> vertex_id_and_stop_name_;
	std::unordered_map < size_t, std::pair<std::string, std::string>> vertex_id_and_bus_name_;
	graph::DirectedWeightedGraph<double> graph_;
	std::vector<json::Node> items;

	void BaseRequest_AddBus();
	void BaseRequests_AddStop(json::Dict stop);

	void SetDistance();
	void Transform(const json::Array &stops_input, std::vector<std::string> &stops_output);

	void StatRequests_PrintBusRequest(json::Dict bus_request);
	void StatRequests_PrintStopRequest(json::Dict stop_request);
	void StatRequests_PrintRouteRequest(json::Dict route_request, graph::Router<double>& router);
	void StatRequests_PrintMapRequests(json::Dict map_request);

	void MakeRenderSettings(json::Dict rendering_settings);
	svg::Color RenderRequests_RgbOrRgba(json::Array color);

	void MakeMap();
	void AddStopsToMap();
	void AddBusesToMap();

	void MakeRouteSettings(json::Dict rendering_settings);
	graph::Router<double> MakeRouter();
};