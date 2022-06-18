#include "svg.h"

namespace svg 
{

	using namespace std::literals;

	void Object::Render(const RenderContext& context) const {
		context.RenderIndent();

		// Делегируем вывод тега своим подклассам
		RenderObject(context);

		context.out << std::endl;
	}

	// ---------- Circle ------------------

	Circle& Circle::SetCenter(Point center) {
		center_ = center;
		return *this;
	}

	Circle& Circle::SetRadius(double radius) {
		radius_ = radius;
		return *this;
	}

	void Circle::RenderObject(const RenderContext& context) const {
		auto& out = context.out;
		out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
		out << "r=\""sv << radius_ << "\" "sv;
		out << "/>"sv;
	}

	Polyline& Polyline::AddPoint(Point point)
	{
		points.push_back(point);
		return *this;
	}

	void Polyline::RenderObject(const RenderContext& context) const {
		auto& out = context.out;
		out << "<polyline points=\""sv << points[0].x << "," << points[0].y;
		for (int i = 1; i < static_cast<int>(points.size()); i++)
		{
			out << " " << points[i].x << "," << points[i].y;
		}
		out << "\" />"sv;
	}

	Text& Text::SetPosition(Point pos)
	{
		pos_ = pos;
		return *this;
	}

	Text& Text::SetOffset(Point offset)
	{
		offset_ = offset;
		return *this;
	}

	Text& Text::SetFontSize(uint32_t size)
	{
		size_ = size;
		return *this;
	}

	Text& Text::SetFontFamily(std::string font_family)
	{
		font_family_ = font_family;
		return *this;
	}

	Text& Text::SetFontWeight(std::string font_weight)
	{
		font_weight_ = font_weight;
		return *this;
	}

	Text& Text::SetData(std::string data)
	{
		data_ = data;
		return *this;
	}

	void Text::RenderObject(const RenderContext& context) const {
		auto& out = context.out;
		out << "<text x=\""sv << pos_.x << "\" y=\""sv << pos_.y
			<< "\" dx=\""sv << offset_.x
			<< "\" dy=\""sv << offset_.y
			<< "\" font-size=\""sv << size_
			<< "\" font-family=\""sv << font_family_
			<< "\" font-weight=\""sv << font_weight_ << "\""
			<< ">"sv << data_
			<< "</text>";
	}

	void Document::AddPtr(std::unique_ptr<Object>&& obj)
	{
		objects_.push_back(move(obj));
	}

	void Document::Render(std::ostream& out) const 
	{
		out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << std::endl;
		out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv << std::endl;
		RenderContext r_c(out);
		for (auto i = 0; i != static_cast<int>(objects_.size()); ++i) {
			objects_[i]->Render(r_c);
		}
		out << "</svg>"sv;
	}
}  // namespace svg