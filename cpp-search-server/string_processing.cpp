#include "string_processing.h"

using namespace std;

vector<string_view> SplitIntoWords(const string_view &text)
{
    vector<string_view> words;
    string word;
    for (const char c : text)
    {
        if (c == ' ')
        {
            if (!word.empty())
            {
                words.push_back(word);
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