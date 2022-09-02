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

	void MapRenderer::AddBusNameOnMap(Bus* bus)
	{
		AddBusNameOnStop(bus, 0u);
		size_t last_stop = bus->bus.size() - 1;
			if (bus->bus[0]->name_stop == bus->bus[last_stop]->name_stop)
				AddBusNameOnStop(bus, last_stop);
	}

	void MapRenderer::AddBusNameOnStop(Bus* bus, size_t last_or_first_stop)
	{
		svg::Text text;
		text.SetPosition(SP(bus->bus[last_or_first_stop]->coor));
		text.SetOffset({ settings.bus_label_offset[0], 
			settings.bus_label_offset[1] });
		text.SetFontSize(settings.bus_label_font_size);
		text.SetFontFamily("Verdana");
		text.SetFontWeight("bold");
		text.SetData(bus->name_bus);
		text.SetFillColor(settings.color_palette[number_of_color]);

		out_doc.Add(text);
	}

	void MapRenderer::AddSubstrateForBusName(Bus* bus, size_t last_or_first_stop)
	{
		svg::Text text;
		text.SetPosition(SP(bus->bus[last_or_first_stop]->coor));
		text.SetOffset({ settings.bus_label_offset[0],
			settings.bus_label_offset[1] });
		text.SetFontSize(settings.bus_label_font_size);
		text.SetFontFamily("Verdana");
		text.SetFontWeight("bold");
		text.SetData(bus->name_bus);
		text.SetFillColor(settings.underlayer_color);
		text.SetStrokeWidth(settings.underlayer_width);
		text.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
		text.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

		out_doc.Add(text);
	}

	void MapRenderer::AddCircleStop()
	{

	}

	void MapRenderer::AddStopName(string stop_name)
	{

	}
}