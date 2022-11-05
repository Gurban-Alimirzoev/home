#include "serialization.h"

void SerializatorDB::SerializeStops()
{
	for (const auto& stop : db_.GetAllStops())
	{
		transport_catalogue_serialize::Stop buf_stop;
		transport_catalogue_serialize::Coordinates buf_coordinates;
		buf_coordinates.set_lat(stop.coor.lat);
		buf_coordinates.set_lng(stop.coor.lng);
		buf_stop.set_name_stop(stop.name_stop);
		*buf_stop.mutable_coor() = buf_coordinates;
		*db_ser_.add_stops() = buf_stop;
	}
}

void SerializatorDB::SerializeBuses()
{
	for (const auto& bus : db_.GetAllBuses())
	{
		transport_catalogue_serialize::Bus buf_bus;
		buf_bus.set_name(bus.name_bus);
		for (auto stop : bus.bus)
		{
			buf_bus.add_stops(stop->name_stop);
		}
		*db_ser_.add_buses()= buf_bus;
	}
}

void SerializatorDB::SerializeDistance()
{
	for (const auto& distance : db_.GetAllEarthDistances())
	{
		transport_catalogue_serialize::Distance buf_distance;
		buf_distance.set_from(distance.first.first->name_stop);
		buf_distance.set_to(distance.first.second->name_stop);
		buf_distance.set_distance(distance.second);
		*db_ser_.add_distance() = buf_distance;
	}
}
void SerializatorDB::SerializeDB(const std::string& filename)
{
	std::ofstream out(filename, std::ios::binary);
	SerializeStops();
	SerializeBuses();
	SerializeDistance();
	db_ser_.SerializeToOstream(&out);
}

void DeserializatorDB::StartDeserializeDB(const std::string& filename)
{
	std::ifstream in(filename, std::ios::binary);
	db_ser_.ParseFromIstream(&in);
}

void DeserializatorDB::DeserializeStops()
{
	for (int i = 0; i < db_ser_.stops().size(); ++i)
	{
		auto stop_ser = db_ser_.stops(i);
		db_.AddStop(stop_ser.name_stop(), { stop_ser.coor().lat(), stop_ser.coor().lng() });
	}
}

void DeserializatorDB::DeserializeBuses()
{

	for (int i = 0; i < db_ser_.buses().size(); ++i)
	{
		auto bus_ser = db_ser_.buses(i);
		std::vector<std::string> stops;
		for (const auto& stop_ser : bus_ser.stops())
		{
			stops.push_back(stop_ser);
		}
		db_.AddBus(bus_ser.name(), stops);
	}
}

void DeserializatorDB::DeserializeDistance()
{
	for (int i = 0; i < db_ser_.distance_size(); ++i)
	{
		const transport_catalogue::Stop* stop_from = db_.FindStop(db_ser_.distance(i).from());
		const transport_catalogue::Stop* stop_to = db_.FindStop(db_ser_.distance(i).to());
		double distance = db_ser_.distance(i).distance();
		db_.SetEarthDistance({ stop_from->name_stop , stop_to->name_stop }, distance);
	}
}