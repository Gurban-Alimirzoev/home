#include "string_processing.h"

using namespace std;
using namespace std::literals;

vector<string_view> SplitIntoWords(const string_view text)
{
	vector<string_view> words;
	string word;// , text_string(text);

	for (const char c : text)
	{
		if (c == ' ')
		{
			if (!word.empty())
			{
				string_view var{ word };
				words.push_back(var);
				word.clear();
			}
		}
		else
		{
			word += c;
		}
	}

	if (!word.empty())
	{
		words.push_back(word);
	}

	return words;
}