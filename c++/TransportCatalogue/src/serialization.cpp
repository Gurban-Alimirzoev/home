#include "serialization.h"

using namespace std;
using namespace renderer;

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
			buf_bus.add_stops_in_bus(stop->name_stop);
		}
		buf_bus.set_is_roundtrip(bus.is_roundtrip);
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

void SerializatorDB::SerealizeRenderSettings()
{
	transport_catalogue_serialize::RenderSettings buf_render_settings;
	transport_catalogue_serialize::Point buf_point;
	buf_render_settings.set_width(render_settings_.width);
	buf_render_settings.set_height(render_settings_.height);
	buf_render_settings.set_padding(render_settings_.padding);
	buf_render_settings.set_line_width(render_settings_.line_width);
	buf_render_settings.set_stop_radius(render_settings_.stop_radius);
	buf_render_settings.set_bus_label_font_size(render_settings_.bus_label_font_size);
	buf_render_settings.set_stop_label_font_size(render_settings_.stop_label_font_size);
	buf_render_settings.set_underlayer_width(render_settings_.underlayer_width);

	buf_point.set_x(render_settings_.bus_label_offset[0]);
	buf_point.set_y(render_settings_.bus_label_offset[1]);
	*buf_render_settings.mutable_bus_label_offset() = buf_point;

	buf_point.set_x(render_settings_.stop_label_offset[0]);
	buf_point.set_y(render_settings_.stop_label_offset[1]);
	*buf_render_settings.mutable_stop_label_offset() = buf_point;

	*buf_render_settings.mutable_underlayer_color() = SerealizeColor(render_settings_.underlayer_color);

	for (const auto& color : render_settings_.color_palette)
	{
		*buf_render_settings.add_color_palette() = SerealizeColor(color);
	}

	*db_ser_.mutable_render_settings() = buf_render_settings;
}

transport_catalogue_serialize::Color SerializatorDB::SerealizeColor(const svg::Color& color)
{
	transport_catalogue_serialize::Color buf_color;
	transport_catalogue_serialize::Rgba buf_rgba;

	if (std::holds_alternative<svg::Rgb>(color))
	{
		svg::Rgb rgb = std::get<svg::Rgb>(color);
		buf_rgba.set_blue(rgb.blue);
		buf_rgba.set_green(rgb.green);
		buf_rgba.set_red(rgb.red);
		*buf_color.mutable_rgba() = buf_rgba;
	}
	else if (std::holds_alternative<svg::Rgba>(color))
	{
		svg::Rgba rgba = std::get<svg::Rgba>(color);
		buf_rgba.set_blue(rgba.blue);
		buf_rgba.set_green(rgba.green);
		buf_rgba.set_red(rgba.red);
		buf_rgba.set_opacity(rgba.opacity);
		buf_color.set_is_rgba(true);
		*buf_color.mutable_rgba() = buf_rgba;
	}
	else if (std::holds_alternative<std::string>(color))
	{
		std::string name = std::get<std::string>(color);
		*buf_color.mutable_name() = name;
	}
	return buf_color;
}

void SerializatorDB::SerealizeRoutingSettings()
{
	transport_catalogue_serialize::RoutingSettings buf_routing_settings;
	buf_routing_settings.set_bus_velocity(route_settings_.bus_velocity);
	buf_routing_settings.set_bus_wait_time(route_settings_.bus_wait_time);
	*db_ser_.mutable_routing_settings() = buf_routing_settings;
}

/*void SerializatorDB::SerealizeTransportRouter()
{
	transport_catalogue_serialize::TransportRouter tr_;

	transport_catalogue_serialize::Pair_stop_name_and_vertex_id stop_name_and_vertex_id;
	for (const auto& stop_and_id: tr_router_.GetStopAndVertexID())
	{
		stop_name_and_vertex_id.set_key(stop_and_id.first);
		stop_name_and_vertex_id.set_value(stop_and_id.second);
		*tr_.add_stop_name_and_vertex_id() = stop_name_and_vertex_id;
	}

	transport_catalogue_serialize::Pair_vertex_id_and_bus_name_ vertex_id_and_name;
	transport_catalogue_serialize::BusRoute bus_route;
	for (const auto& vertex_and_name : tr_router_.GetVertexIDAndBus())
	{
		bus_route.set_bus_name(vertex_and_name.second.bus_name);
		bus_route.set_start(vertex_and_name.second.start);
		bus_route.set_finish(vertex_and_name.second.finish);
		bus_route.set_span_count(vertex_and_name.second.span_count);

		vertex_id_and_name.set_key(vertex_and_name.first);
		*vertex_id_and_name.mutable_value() = bus_route;

		*tr_.add_vertex_id_and_bus_name_() = vertex_id_and_name;
	}
	*db_ser_.mutable_router() = tr_;
}*/

void SerializatorDB::SerializeDB(const string& filename)
{
	ofstream out(filename, ios::binary);
	SerializeStops();
	SerializeBuses();
	SerializeDistance();
	SerealizeRenderSettings();
	SerealizeRoutingSettings();
	db_ser_.SerializeToOstream(&out);
}

void DeserializatorDB::StartDeserializeDB(const string& filename)
{
	ifstream in(filename, ios::binary);
	db_ser_.ParseFromIstream(&in);
}

void DeserializatorDB::DeserializeStops()
{
	for (int i = 0; i < db_ser_.stops().size(); ++i)
	{
		auto stop_ser = db_ser_.stops(i);
		db_.AddStop(stop_ser.name_stop(), { stop_ser.coor().lat(), stop_ser.coor().lng() });
		tr_router.SetStop(stop_ser.name_stop());
	}
}

void DeserializatorDB::DeserializeBuses()
{

	for (int i = 0; i < db_ser_.buses().size(); ++i)
	{
		auto bus_ser = db_ser_.buses(i);
		vector<string> stops(bus_ser.stops_in_bus().begin(), bus_ser.stops_in_bus().end());
		db_.AddBus(bus_ser.name(), stops, bus_ser.is_roundtrip());
		tr_router.SetBus(bus_ser.name(), stops);
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
		//tr_router.SetStop(stop_from->name_stop);
		tr_router.SetStop(stop_to->name_stop);
	}
}

Settings DeserializatorDB::DeserializeRenderSettings()
{
	Settings render_settings_;
	render_settings_.width = db_ser_.render_settings().width();
	render_settings_.height = db_ser_.render_settings().height();
	render_settings_.padding = db_ser_.render_settings().padding();
	render_settings_.line_width = db_ser_.render_settings().line_width();
	render_settings_.stop_radius = db_ser_.render_settings().stop_radius();
	render_settings_.bus_label_font_size = db_ser_.render_settings().bus_label_font_size();
	render_settings_.stop_label_font_size = db_ser_.render_settings().stop_label_font_size();
	render_settings_.underlayer_width = db_ser_.render_settings().underlayer_width();

	render_settings_.bus_label_offset = { db_ser_.render_settings().bus_label_offset().x()
		, db_ser_.render_settings().bus_label_offset().y() };
	render_settings_.stop_label_offset = { db_ser_.render_settings().stop_label_offset().x()
		, db_ser_.render_settings().stop_label_offset().y() };

	render_settings_.underlayer_color = DeserealizeColor(db_ser_.render_settings().underlayer_color());

	for (const auto& color_ser : db_ser_.render_settings().color_palette())
	{
		render_settings_.color_palette.push_back(DeserealizeColor(color_ser));
	}
	return render_settings_;
}

svg::Color DeserializatorDB::DeserealizeColor(const transport_catalogue_serialize::Color& color)
{
	if (!color.name().empty())
	{
		return color.name();
	}
	else if (color.is_rgba())
	{
		return svg::Rgba(color.rgba().red(), color.rgba().green(), color.rgba().blue(), color.rgba().opacity());
	}
	return svg::Rgb(color.rgba().red(), color.rgba().green(), color.rgba().blue());
}

void DeserializatorDB::DeserealizeRoutingSettings()
{
	transport_catalogue::route::Settings router_settings;
	auto routing_settings_ser = db_ser_.routing_settings();
	router_settings.bus_wait_time = routing_settings_ser.bus_wait_time();
	router_settings.bus_velocity = routing_settings_ser.bus_velocity();
	tr_router.SetRoutingSettings(router_settings.bus_wait_time, router_settings.bus_velocity);
}

/*transport_catalogue::route::TransportRouter DeserializatorDB::DeserealizeTransportRouter()
{

	for (const auto& name_and_vertex: db_ser_.router().stop_name_and_vertex_id())
	{
		name_and_vertex.
	}
}*/