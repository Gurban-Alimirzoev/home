#include "map_renderer.h"


using namespace std;
using namespace transport_catalogue;
using namespace geo;
namespace renderer
{

	svg::Document MapRenderer::GetMapDocument() const
	{
		return out_doc;
	}

	void MapRenderer::CalculateSphereProjectorSettings(unordered_map<string_view, Stop*> stopname_to_stop)
	{
		/*double
			min_lat = 0,
			min_lon = 0,
			max_lat = 0,
			max_lon = 0;

		for (auto [stop_name, stop_data] : stopname_to_stop)
			{
				if (stop_data->coor.lat < min_lat)
					min_lat = stop_data->coor.lat;
				if (stop_data->coor.lng < min_lon)
					min_lat = stop_data->coor.lat;
				if (stop_data->coor.lat > max_lat)
					min_lat = stop_data->coor.lat;
				if (stop_data->coor.lng > max_lon)
					min_lat = stop_data->coor.lat;
			}*/

		SphereProjector(settings.width, settings.height, settings.padding);
	}

	void MapRenderer::AddBus()
	{

	}
}