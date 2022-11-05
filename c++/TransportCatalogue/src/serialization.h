#pragma once
#include <vector>
#include <string>
#include <fstream>

#include "transport_catalogue.h"
#include <transport_catalogue.pb.h>

class SerializatorDB
{
public:
	SerializatorDB(const transport_catalogue::TransportCatalogue& db)
		:db_(db)
	{}

	void SerializeDB(const std::string& filename);
private:
	const transport_catalogue::TransportCatalogue& db_;
	transport_catalogue_serialize::TransportCatalogue db_ser_;

	void SerializeStops();
	void SerializeBuses();
	void SerializeDistance();
};

class DeserializatorDB
{
public:
	DeserializatorDB(transport_catalogue::TransportCatalogue& db)
		:db_(db)
	{}

	void StartDeserializeDB(const std::string& filename);
	void DeserializeStops();
	void DeserializeBuses();
	void DeserializeDistance();
private:
	transport_catalogue::TransportCatalogue& db_;
	transport_catalogue_serialize::TransportCatalogue db_ser_;
};