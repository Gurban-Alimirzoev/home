#define _USE_MATH_DEFINES
#include "svg.h" // Объявления классов библиотеки должны быть расположены в файле svg.h

#include <cmath>
#include <cassert>

namespace {
    svg::Polyline CreateStar(svg::Point center, double outer_rad, double inner_rad, int num_rays) {
        using namespace svg;
        Polyline polyline;
        for (int i = 0; i <= num_rays; ++i) {
            double angle = 2 * M_PI * (i % num_rays) / num_rays;
            polyline.AddPoint({ center.x + outer_rad * sin(angle), center.y - outer_rad * cos(angle) });
            if (i == num_rays) {
                break;
            }
            angle += M_PI / num_rays;
            polyline.AddPoint({ center.x + inner_rad * sin(angle), center.y - inner_rad * cos(angle) });
        }
        return polyline;
    }
}

namespace shapes {

    class Triangle : public svg::Drawable {
    public:
        Triangle(svg::Point p1, svg::Point p2, svg::Point p3)
            : p1_(p1)
            , p2_(p2)
            , p3_(p3) {
        }

        // Реализует метод Draw интерфейса svg::Drawable
        void Draw(svg::ObjectContainer& container) const override {
            container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
        }

    private:
        svg::Point p1_, p2_, p3_;
    };

    class Star : public svg::Drawable {
    public:
        Star(svg::Point center, double outer_rad, double inner_rad, int num_rays)
            : star(CreateStar(center, outer_rad, inner_rad, num_rays).SetFillColor("red").SetStrokeColor("black"))
        {}

        void Draw(svg::ObjectContainer& container) const override {
            container.Add(star);
        }

    private:
        svg::Polyline star;
    };

    class Snowman : public svg::Drawable {
    public:
        Snowman(svg::Point center, double first_radius)
        {
            first_circle.SetCenter(center).SetRadius(first_radius).SetFillColor("rgb(240,240,240)").SetStrokeColor("black");
            second_circle.SetCenter({ center.x, center.y + 2 * first_radius }).SetRadius(first_radius * 1.5).SetFillColor("rgb(240,240,240)").SetStrokeColor("black");
            third_circle.SetCenter({ center.x, center.y + 5 * first_radius }).SetRadius(first_radius * 2).SetFillColor("rgb(240,240,240)").SetStrokeColor("black");
        }

        void Draw(svg::ObjectContainer& container) const override {
            container.Add(third_circle);
            container.Add(second_circle);
            container.Add(first_circle);
        }


    private:
        svg::Circle first_circle, second_circle, third_circle;
    };

}

template <typename DrawableIterator>
void DrawPicture(DrawableIterator begin, DrawableIterator end, svg::ObjectContainer& target) {
    for (auto it = begin; it != end; ++it) {
        (*it)->Draw(target);
    }
}

template <typename Container>
void DrawPicture(const Container& container, svg::ObjectContainer& target) {
    using namespace std;
    DrawPicture(begin(container), end(container), target);
}

// Выполняет линейную интерполяцию значения от from до to в зависимости от параметра t
uint8_t Lerp(uint8_t from, uint8_t to, double t) {
    return static_cast<uint8_t>(std::round((to - from) * t + from));
}

// Выполняет линейную интерполяцию Rgb цвета от from до to в зависимости от параметра t
svg::Rgb Lerp(svg::Rgb from, svg::Rgb to, double t) {
    return { Lerp(from.red, to.red, t), Lerp(from.green, to.green, t), Lerp(from.blue, to.blue, t) };
}

int main() {
    using namespace svg;
    using namespace shapes;
    using namespace std;
    using namespace std::literals;


    svg::Rgba rgba{ 100, 20, 50, 0.3 };
    assert(rgba.red == 100);
    assert(rgba.green == 20);
    assert(rgba.blue == 50);
    assert(rgba.opacity == 0.3);

    svg::Rgba color;
    assert(color.red == 0 && color.green == 0 && color.blue == 0 && color.opacity == 1.0);
}