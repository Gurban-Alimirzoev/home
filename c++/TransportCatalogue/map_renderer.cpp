#include "map_renderer.h"


using namespace std;
using namespace transport_catalogue;
using namespace geo;

namespace renderer
{

	void MapRenderer::AddPoints(
		deque<Stop*> bus)
	{
		svg::Polyline line;
		for_each(
			bus.begin(),
			bus.end(),
			[this, &line](auto name, Stop* data)
			{
				line.AddPoint({ data->coor.lat, data->coor.lng) });
		});

		out_doc.Add(line);
	}

	void MapRenderer::RenderMap()
	{
		SphereProjector SP(
			Points.begin(),
			Points.end(),
			settings.width
			, settings.height
			, settings.padding);
	}
}