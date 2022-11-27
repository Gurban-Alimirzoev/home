#include "sheet.h"

using namespace std;
using namespace std::literals;

/*template <typename Value>
Value findMaxValuePair(
	unordered_set<Value> const& _map)
{
	return *max_element(_map.begin(), _map.end(),
		[](const Value& p1,
			const Value& p2)
		{
			return p1 < p2;
		});
}*/

struct OstreamValuePrinter {
	ostream& out;

	void operator()(string value_) const {
		out << value_;
	}
	void operator()(double value_) const {
		out << value_;
	}
	void operator()(FormulaError error) const {
		out << error;
	}
};

bool Comp(const Position& p1, const Position& p2)
{
	return std::tie(p1.row, p1.col) < std::tie(p2.row, p2.col);
}

Sheet::~Sheet() {
	/*int map_size = static_cast<int>(pos_and_cells.size());
	for (int i= 0; i < map_size; i++)
	{
		ClearCell(pos_and_cells[i]);
	}*/
}

void Sheet::SetCell(Position pos, string text) {
	if (!pos.IsValid())
		throw InvalidPositionException("");

	auto iter = pos_and_cells.find(pos);
	if (iter != pos_and_cells.end())
	{
		ReplaceCell(pos, text);
	}
	else
	{
		unique_ptr<CellInterface> cell = make_unique<Cell>();
		cell->Set(text);
		pos_and_cells.insert({ pos, move(cell) });
		all_pos.insert(pos);
		//cells_in_row.insert({ pos, move(cell) });
	}


	/*shared_ptr<CellInterface> cell = make_unique<Cell>();
	cell->Set(text);
	pos_and_cells.insert({ pos, cell });
	cells_and_pos.insert({ cell, pos });
	cells_in_row.insert({ pos, cell });*/
}

void Sheet::ReplaceCell(Position pos, string text)
{
	pos_and_cells[pos]->Set(text);
}
const CellInterface* Sheet::GetCell(Position pos) const {
	auto iter = pos_and_cells.find(pos);
	if (iter == pos_and_cells.end())
		return nullptr;
	return iter->second.get();
}
CellInterface* Sheet::GetCell(Position pos) {
	auto iter = pos_and_cells.find(pos);
	if (iter == pos_and_cells.end())
		return nullptr;
	return iter->second.get();
}

void Sheet::ClearCell(Position pos) {
	auto iter = pos_and_cells.find(pos);
	if (iter != pos_and_cells.end())
	{
		all_pos.erase(find(all_pos.begin(), all_pos.end(), pos));
		pos_and_cells[pos]->~CellInterface();
		pos_and_cells.erase(pos);
	}
}

Size Sheet::GetPrintableSize() const {
	if (all_pos.empty())
		return Size();
	auto max_pos = *max_element(all_pos.begin(), all_pos.end(),
		[](const Position& p1,
			const Position& p2)
		{
			return p1 < p2;
		});;
	return { max_pos.row + 1, max_pos.col + 1 };
}

void Sheet::PrintValues(ostream& output) const {
	int number_row = 1;
	int cells_row = static_cast<int>(pos_and_cells.size());
	int check = 0;

	for (const auto& pos : all_pos)
	{
		if (number_row <= pos.row)
		{
			for (; number_row <= pos.row; number_row++) {}
			output << '\n';
		}

		ostringstream strm;
		visit(OstreamValuePrinter{ strm }, pos_and_cells.at(pos)->GetValue());
		output << strm.str() << ((++check < cells_row) ? '\t' : '\n');
	}
}
void Sheet::PrintTexts(ostream& output) const {
	int number_row = 1;
	int cells_row = static_cast<int>(pos_and_cells.size());
	int check = 0;

	for (const auto& pos : all_pos)
	{
		if (number_row <= pos.row)
		{
			for (; number_row <= pos.row; number_row++) {}
			output << '\n';
		}
		output << pos_and_cells.at(pos)->GetText() << ((++check < cells_row) ? '\t' : '\n');
	}
}

unique_ptr<SheetInterface> CreateSheet() {
	return make_unique<Sheet>();
}
