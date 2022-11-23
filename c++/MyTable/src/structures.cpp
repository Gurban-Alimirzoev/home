#include "common.h"

#include <cctype>
#include <sstream>

using namespace std;

const int LETTERS = 26;
const int MAX_POSITION_LENGTH = 17;
const int MAX_POS_LETTER_COUNT = 3;

static const int number_of_english_words = 26;

const Position Position::NONE = {-1, -1};

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
	return this->row < MAX_ROWS&& this->row >= 0 && this->col < MAX_COLS&& this->col >= 0;
}

string Position::ToString() const 
{
	if (!IsValid())
	{
		return;
	}

	string result;
	int first_word = this->col;
	while (first_word > number_of_english_words)
	{
		first_word %= number_of_english_words;
		result += char(first_word);
	}
	result += char(first_word);
	result += to_string(this->row + 1);
	return result;
}

Position Position::FromString(string_view str) 
{	
	bool col = false;
	bool row = false;
	Position pos;
	for (char c : str)
	{
		if (c > '@' && c < '[' && !row)
		{
			pos.col += c - '@';
		}
	}
}