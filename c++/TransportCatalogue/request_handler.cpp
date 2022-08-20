#include "request_handler.h"

/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */

using namespace std;

namespace transport_catalogue
{

	optional<BusInfo> RequestHandler::GetBusStat(const string_view& bus_name) const
	{
		return db_.GetBusInfo(bus_name);
	}

	const unordered_set<Bus*> RequestHandler::GetBusesByStop(const string_view& stop_name) const
	{
		return db_.GetAllBusOnStop(stop_name);
	}
}