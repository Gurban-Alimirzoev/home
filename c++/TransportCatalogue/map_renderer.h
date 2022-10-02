#pragma once
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>
#include <deque>
#include <utility>
#include <unordered_map>
#include <string_view>

#include "svg.h"
#include "geo.h"
#include "domain.h"

namespace renderer
{
    class SphereProjector
    {
    public:
        SphereProjector() = default;

        // points_begin и points_end задают начало и конец интервала элементов geo::Coordinates
        template <typename PointInputIt>
        void CalculateCoef(PointInputIt points_begin, PointInputIt points_end,
                           double max_width, double max_height, double padding)
        {
            padding_ = padding;
            // Если точки поверхности сферы не заданы, вычислять нечего
            if (points_begin == points_end)
            {
                return;
            }

            // Находим точки с минимальной и максимальной долготой
            const auto [left_it, right_it] = std::minmax_element(
                points_begin, points_end,
                [](auto lhs, auto rhs)
                { return lhs.y < rhs.y; });
            min_lon_ = left_it->y;
            const double max_lon = right_it->y;

            // Находим точки с минимальной и максимальной широтой
            const auto [bottom_it, top_it] = std::minmax_element(
                points_begin, points_end,
                [](auto lhs, auto rhs)
                { return lhs.x < rhs.x; });
            const double min_lat = bottom_it->x;
            max_lat_ = top_it->x;

            // Вычисляем коэффициент масштабирования вдоль координаты x
            std::optional<double> width_zoom;
            if (!IsZero(max_lon - min_lon_))
            {
                width_zoom = (max_width - 2 * padding) / (max_lon - min_lon_);
            }

            // Вычисляем коэффициент масштабирования вдоль координаты y
            std::optional<double> height_zoom;
            if (!IsZero(max_lat_ - min_lat))
            {
                height_zoom = (max_height - 2 * padding) / (max_lat_ - min_lat);
            }

            if (width_zoom && height_zoom)
            {
                // Коэффициенты масштабирования по ширине и высоте ненулевые,
                // берём минимальный из них
                zoom_coeff_ = std::min(*width_zoom, *height_zoom);
            }
            else if (width_zoom)
            {
                // Коэффициент масштабирования по ширине ненулевой, используем его
                zoom_coeff_ = *width_zoom;
            }
            else if (height_zoom)
            {
                // Коэффициент масштабирования по высоте ненулевой, используем его
                zoom_coeff_ = *height_zoom;
            }
        }
        // Проецирует широту и долготу в координаты внутри SVG-изображения
        svg::Point operator()(geo::Coordinates coords) const
        {
            return {
                (coords.lng - min_lon_) * zoom_coeff_ + padding_,
                (max_lat_ - coords.lat) * zoom_coeff_ + padding_};
        }

        const double EPSILON = 1e-6;
        bool IsZero(double value)
        {
            return std::abs(value) < EPSILON;
        }

    private:
        double padding_ = 0;
        double min_lon_ = 0;
        double max_lat_ = 0;
        double zoom_coeff_ = 0;
    };

    struct Settings
    {
        int
            bus_label_font_size = 0,
            stop_label_font_size = 0;
        double
            width = 0,
            height = 0,
            padding = 0,
            line_width = 0,
            stop_radius = 0,
            underlayer_width = 0;
        std::vector<double>
            bus_label_offset = {0, 0},
            stop_label_offset = {0, 0};
        svg::Color underlayer_color;
        std::vector<svg::Color> color_palette;
    };

    class MapRenderer
    {
    public:
        MapRenderer() = default;
        MapRenderer(Settings settings_)
            : render_settings(settings_)
        {
        }
        void SetSettings(Settings settings_);
        void SavePoints(svg::Point);
        void MakeSphereProjector();
        void AddBusLine(std::vector<transport_catalogue::Stop *> bus);
        void AddBusNameOnMap(transport_catalogue::Bus *bus, bool is_roundtrip);
        void AddCircleStops();
        void AddStopName(transport_catalogue::Stop &stop);
        void RestartNumberOfColor();
        void RenderMap(std::ostream &out);

    private:
        Settings render_settings;
        SphereProjector SP;
        std::vector<svg::Object> polylines;
        svg::Document out_doc;
        std::vector<svg::Point> all_points;
        int number_of_color = 0;


        void AddBusNameOnStop(transport_catalogue::Bus* bus, size_t last_or_first_stop);
        void AddSubstrateForBusName(transport_catalogue::Bus* bus, size_t last_or_first_stop);
        void AddSubstrateForStopName(transport_catalogue::Stop& stop);
        void AddStopNameOnMap(transport_catalogue::Stop& stop);
        void PlusNumberOfColor();
    };
}