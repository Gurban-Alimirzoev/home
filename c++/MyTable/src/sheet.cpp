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
	}
}

void Sheet::ReplaceCell(Position pos, string text)
{
	pos_and_cells[pos]->Set(text);
}
const CellInterface* Sheet::GetCell(Position pos) const {
	if (!pos.IsValid())
		throw InvalidPositionException("");
	auto iter = pos_and_cells.find(pos);
	if (iter == pos_and_cells.end())
		return nullptr;
	return iter->second.get();
}
CellInterface* Sheet::GetCell(Position pos) {
	if (!pos.IsValid())
		throw InvalidPositionException("");
	auto iter = pos_and_cells.find(pos);
	if (iter == pos_and_cells.end())
		return nullptr;
	return iter->second.get();
}

void Sheet::ClearCell(Position pos) {
	if (!pos.IsValid())
		throw InvalidPositionException("");

	auto iter = pos_and_cells.find(pos);
	if (iter != pos_and_cells.end())
	{
		all_pos.erase(find(all_pos.begin(), all_pos.end(), pos));
		pos_and_cells.erase(iter);
	}

}

Size Sheet::GetPrintableSize() const {
	if (all_pos.empty())
		return Size();
	int max_row = 0;
	int max_col = 0;
	for (auto pos : all_pos)
	{
		max_row = max_row > pos.row ? max_row : pos.row;
		max_col = max_col > pos.col ? max_col : pos.col;
	}
	return { max_row + 1, max_col + 1 };
}

void Sheet::PrintValues(ostream& output) const {
	if (all_pos.empty())
		return;
	Position current_pos;
	auto [max_r, max_c] = GetPrintableSize();
	Position max_position({ max_r, max_c });

	for (const auto& pos : all_pos)
	{
		StepInSheet(current_pos, pos, max_position, output);

		ostringstream strm;
		visit(OstreamValuePrinter{ strm }, pos_and_cells.at(pos)->GetValue());
		output << strm.str();
	}
	LastStepInSheet(current_pos, max_position, output);
}

void Sheet::PrintTexts(ostream& output) const {
	if (all_pos.empty())
		return;
	Position current_pos;
	auto [max_r, max_c] = GetPrintableSize();
	Position max_position({ max_r, max_c});
	for (const auto& pos : all_pos)
	{
		StepInSheet(current_pos, pos, max_position, output);
		output << pos_and_cells.at(pos)->GetText();
	}
	LastStepInSheet(current_pos, max_position, output);
}

void Sheet::StepInSheet(Position& current_pos,const Position& next_pos,const Position& max_position, std::ostream& output) const
{
	if (current_pos.row != next_pos.row)
	{
		if (current_pos.col + 1 != max_position.col)
		{
			PrintT(output);
		}
		for (; current_pos.row != next_pos.row; current_pos.row++) {
			PrintN(output);
		}
		current_pos.col = 0;
	}

	if (current_pos.col != next_pos.col)
	{
		for (; current_pos.col != next_pos.col; current_pos.col++)
		{
			PrintT(output);
		}
	}
}

void Sheet::LastStepInSheet(Position& current_pos, Position& max_position, std::ostream& output) const
{
	if (current_pos.col + 1 != max_position.col)
	{
		PrintT(output);
	}
	PrintN(output);
}

void Sheet::PrintT(ostream& output) const
{
	output << '\t';
}

void Sheet::PrintN(ostream& output) const
{
	output << '\n';
}

unique_ptr<SheetInterface> CreateSheet() {
	return make_unique<Sheet>();
}
