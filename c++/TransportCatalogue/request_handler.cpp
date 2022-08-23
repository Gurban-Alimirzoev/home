#include "request_handler.h"

using namespace std;

namespace transport_catalogue
{

	optional<BusInfo> RequestHandler::GetBusStat(const string_view& bus_name) const
	{
		return db_.GetBusInfo(bus_name);
	}

	int RequestHandler::GetStopCount(const std::string_view& bus_name) const
	{
		return static_cast<int>(db_.FindBus(bus_name)->bus.size());
	}

	bool RequestHandler::ChekStop(const std::string_view& stop_name) const
	{
		return (db_.FindStop(stop_name) != nullptr);
	}

	bool RequestHandler::ChekBus(const std::string_view& bus_name) const
	{
		return (db_.FindBus(bus_name) != nullptr);
	}

	const unordered_set<Bus*> RequestHandler::GetBusesByStop(const string_view& stop_name) const
	{
		return db_.GetAllBusOnStop(stop_name);
	}

}