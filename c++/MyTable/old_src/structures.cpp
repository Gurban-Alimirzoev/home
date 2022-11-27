#include "common.h"

#include <cctype>
#include <sstream>
#include <charconv>
#include <cmath>
#include <vector>
//using namespace std;

const int LETTERS = 26;
const int MAX_POSITION_LENGTH = 17;
const int MAX_POS_LETTER_COUNT = 3;

const Position Position::NONE = { -1, -1 };

bool IsWord(char c)
{
	return c > '@' && c < '[';
}

bool IsNumber(char c)
{
	return c > '/' && c < ':';
}

void ConvertNumberToColFormat(int& col, std::string& result)
{
	/*if (col >= LETTERS)
	{
		int base_word = col / LETTERS;
		ConvertNumberToColFormat(base_word, result);
		col %= LETTERS;
		result += char(base_word + '@');
		//ConvertNumberToColFormat(col, result);
	}*/

	while (col >= LETTERS)
	{
		int base_word = col / LETTERS;

		if (base_word > LETTERS)
		{
			int cur_word = base_word / LETTERS;
			base_word %= LETTERS;
			result += char(cur_word + '@');
		}

		col %= LETTERS;
		result += char(base_word + '@');
	}
}

// Реализуйте методы:
bool Position::operator==(const Position rhs) const
{
	return this->row == rhs.row && this->col == rhs.col;
}

bool Position::operator<(const Position rhs) const
{
	return this->row * this->col < rhs.row * rhs.col;
}

bool Position::IsValid() const
{
	return this->row < MAX_ROWS && this->row >= 0 && this->col < MAX_COLS && this->col >= 0;
}

std::string Position::ToString() const
{
	if (!IsValid())
	{
		return{};
	}

	std::string result;
	int first_word = (this->col);

	ConvertNumberToColFormat(first_word, result);

	result += char(first_word + 'A');
	result += std::to_string(this->row + 1);
	return result;
}

Position Position::FromString(std::string_view str)
{
	Position pos;
	std::vector<int> cols;

	if (str.size() < 2)
	{
		return NONE;
	}

	int number_of_word = 0;
	for (char c : str)
	{
		if (IsWord(c) && number_of_word < MAX_POS_LETTER_COUNT)
		{
			cols.push_back(c - '@');
			number_of_word++;
		}
		else if (IsWord(c) && number_of_word == MAX_POS_LETTER_COUNT)
		{
			return NONE;
		}
		else if (!IsNumber(c))
		{
			return NONE;
		}
		else if (IsNumber(c))
		{
			break;
		}

	}

	for (int col : cols)
	{
		pos.col += col * std::pow(LETTERS, --number_of_word);
	}
	pos.col--;
	str = str.substr(cols.size());

	for (char c : str)
	{
		if (!IsNumber(c))
		{
			return NONE;
		}
	}

	auto [ptr, ec] {std::from_chars(str.data(), str.data() + str.size(), pos.row)};
	pos.row--;
	return pos.IsValid() && ec == std::errc() ? pos : NONE;
}
