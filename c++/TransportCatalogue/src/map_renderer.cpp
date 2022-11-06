#include "map_renderer.h"

using namespace std;
using namespace transport_catalogue;
using namespace geo;

namespace renderer
{
	void MapRenderer::SetSettings(Settings settings_)
	{
		render_settings = settings_;
	}

	void MapRenderer::MakeSphereProjector()
	{
		SP.CalculateCoef(all_points.begin(), all_points.end(), render_settings.width, render_settings.height, render_settings.padding);
	}

	void MapRenderer::SavePoints(svg::Point p)
	{
		all_points.push_back(p);
	}

	void MapRenderer::RenderMap(std::ostream &out)
	{
		out_doc.Render(out);
	}

	void MapRenderer::AddBusLine(vector<Stop *> bus)
	{
		svg::Polyline line;
		for (Stop *data : bus)
		{
			line.AddPoint(SP(data->coor));
		}
		line.SetFillColor("none");
		line.SetStrokeWidth(render_settings.line_width);
		line.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
		line.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
		line.SetStrokeColor(render_settings.color_palette[number_of_color]);
		PlusNumberOfColor();

		out_doc.Add(line);
	}

	void MapRenderer::AddBusNameOnMap(Bus *bus, bool is_roundtrip)
	{
		AddSubstrateForBusName(bus, 0u);
		AddBusNameOnStop(bus, 0u);
		size_t last_stop_if_not_roundtrip = bus->bus.size() / 2;
		if (!is_roundtrip && bus->bus[0]->name_stop != bus->bus[last_stop_if_not_roundtrip]->name_stop)
		{
			AddSubstrateForBusName(bus, last_stop_if_not_roundtrip);
			AddBusNameOnStop(bus, last_stop_if_not_roundtrip);
		}
		PlusNumberOfColor();
	}

	void MapRenderer::AddBusNameOnStop(Bus *bus, size_t last_or_first_stop)
	{
		svg::Text text;
		text.SetPosition(SP(bus->bus[last_or_first_stop]->coor));
		text.SetOffset({render_settings.bus_label_offset[0],
						render_settings.bus_label_offset[1]});
		text.SetFontSize(render_settings.bus_label_font_size);
		text.SetFontFamily("Verdana");
		text.SetFontWeight("bold");
		text.SetData(bus->name_bus);
		text.SetFillColor(render_settings.color_palette[number_of_color]);

		out_doc.Add(text);
	}

	void MapRenderer::AddSubstrateForBusName(Bus *bus, size_t last_or_first_stop)
	{
		svg::Text text;
		text.SetPosition(SP(bus->bus[last_or_first_stop]->coor));
		text.SetOffset({render_settings.bus_label_offset[0],
						render_settings.bus_label_offset[1]});
		text.SetFontSize(render_settings.bus_label_font_size);
		text.SetFontFamily("Verdana");
		text.SetFontWeight("bold");
		text.SetData(bus->name_bus);
		text.SetFillColor(render_settings.underlayer_color);
		text.SetStrokeColor(render_settings.underlayer_color);
		text.SetStrokeWidth(render_settings.underlayer_width);
		text.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
		text.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

		out_doc.Add(text);
	}

	void MapRenderer::AddCircleStops()
	{
		for (auto point : all_points)
		{
			svg::Circle circle;
			circle.SetCenter(SP(geo::Coordinates({point.x, point.y})));
			circle.SetRadius(render_settings.stop_radius);
			circle.SetFillColor("white");
			out_doc.Add(circle);
		}
	}

	void MapRenderer::AddStopName(Stop &stop)
	{
		AddSubstrateForStopName(stop);
		AddStopNameOnMap(stop);
	}

	void MapRenderer::AddSubstrateForStopName(transport_catalogue::Stop &stop)
	{
		svg::Text text;
		text.SetPosition(SP(stop.coor));
		text.SetOffset({render_settings.stop_label_offset[0],
						render_settings.stop_label_offset[1]});
		text.SetFontSize(render_settings.stop_label_font_size);
		text.SetFontFamily("Verdana");
		text.SetData(stop.name_stop);
		text.SetFillColor(render_settings.underlayer_color);
		text.SetStrokeColor(render_settings.underlayer_color);
		text.SetStrokeWidth(render_settings.underlayer_width);
		text.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
		text.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

		out_doc.Add(text);
	}
	void MapRenderer::AddStopNameOnMap(transport_catalogue::Stop &stop)
	{
		svg::Text text;
		text.SetPosition(SP(stop.coor));
		text.SetOffset({render_settings.stop_label_offset[0],
						render_settings.stop_label_offset[1]});
		text.SetFontSize(render_settings.stop_label_font_size);
		text.SetFontFamily("Verdana");
		text.SetData(stop.name_stop);
		text.SetFillColor("black");

		out_doc.Add(text);
	}

	void MapRenderer::RestartNumberOfColor()
	{
		number_of_color = 0;
	}

	void MapRenderer::PlusNumberOfColor()
	{
		if (number_of_color + 1 == static_cast<int>(render_settings.color_palette.size()))
			number_of_color = 0;
		else
			number_of_color++;
	}

	const Settings MapRenderer::GetSettings() const
	{
		return render_settings;
	}
}