#include "map_renderer.h"

using namespace std;
using namespace transport_catalogue;
using namespace geo;

namespace renderer
{
	void MapRenderer::SetSettings(Settings settings_)
	{
		settings = settings_;
	}

	void MapRenderer::MakeSphereProjector()
	{
		SP.CalculateCoef(all_points.begin()
			, all_points.end()
			, settings.width
			, settings.height
			, settings.padding);
	}

	void MapRenderer::SavePoints(svg::Point p)
	{
		all_points.push_back(p);
	}

	void MapRenderer::RenderMap(std::ostream& out)
	{
		out_doc.Render(out);
	}

	void MapRenderer::AddBusLine(vector<Stop*> bus)
	{
		if (!bus.empty())
		{
			svg::Polyline line;
			for (Stop* data : bus)
			{
				line.AddPoint(SP(data->coor));
			}
			line.SetFillColor("none");
			line.SetStrokeWidth(settings.line_width);
			line.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
			line.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
			line.SetStrokeColor(settings.color_palette[number_of_color]);

			if (number_of_color + 1 == static_cast<int>(settings.color_palette.size()))
				number_of_color = 0;
			else
				number_of_color++;

			out_doc.Add(line);
		}
	}

	void MapRenderer::AddBusName(string_view& bus_name, bool is_roundtrip)
	{

	}

	void MapRenderer::AddCircleStop()
	{

	}

	void MapRenderer::AddStopName(string_view& stop_name)
	{

	}
}