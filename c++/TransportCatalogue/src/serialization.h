#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "transport_catalogue.h"
#include "map_renderer.h"
#include "transport_router.h"
#include "graph.h"

#include <transport_catalogue.pb.h>
#include <map_renderer.pb.h>
#include <graph.pb.h>
//#include "transport_router.pb.h"

class SerializatorDB
{
public:
	SerializatorDB(const transport_catalogue::TransportCatalogue& db
		, const renderer::Settings& render_settings
		, const transport_catalogue::route::Settings& route_settings)

		:db_(db), 
		render_settings_(render_settings),
		route_settings_(route_settings)
	{}

	void SerializeDB(const std::string& filename);
private:

	const transport_catalogue::TransportCatalogue& db_;
	const renderer::Settings& render_settings_;
	const transport_catalogue::route::Settings route_settings_;
	transport_catalogue_serialize::TransportCatalogue db_ser_;

	void SerializeStops();
	void SerializeBuses();
	void SerializeDistance();
	void SerealizeRenderSettings();
	transport_catalogue_serialize::Color SerealizeColor(const svg::Color& color);
	void SerealizeRoutingSettings();
	//void SerealizeTransportRouter();
};

class DeserializatorDB
{
public:
	DeserializatorDB(transport_catalogue::TransportCatalogue& db
		, renderer::Settings& render_settings
		, transport_catalogue::route::TransportRouter& router)

		:db_(db),
		render_settings_(render_settings),
		tr_router(router)
	{}

	void StartDeserializeDB(const std::string& filename);
	void DeserializeStops();
	void DeserializeBuses();
	void DeserializeDistance();
	renderer::Settings DeserializeRenderSettings();
	svg::Color DeserealizeColor(const transport_catalogue_serialize::Color& color);
	void DeserealizeRoutingSettings();
	//transport_catalogue::route::TransportRouter DeserealizeTransportRouter();

private:
	transport_catalogue::TransportCatalogue& db_;
	renderer::Settings& render_settings_;
	transport_catalogue::route::TransportRouter& tr_router;
	transport_catalogue_serialize::TransportCatalogue db_ser_;
	
};