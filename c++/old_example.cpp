#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <deque>
#include <list>
#include <sstream>
#include <string>
#include <future>
#include <execution>
#include <utility>
#include <string_view>
using namespace std;

using KeyWords = set<string, less<>>;

struct Stats
{
    map<string, int> word_frequences;

    Stats &operator+=(const Stats &other)
    {
        for (const auto &p : other.word_frequences)
        {
            word_frequences[p.first] += p.second;
        }
        return *this;
    }
};

Stats GetStatsWords(const KeyWords &key_words, const string str)
{
    Stats result;
    stringstream streamData(str);
    string word;
    list<string> vec_text;
    while (getline(streamData, word, ' '))
    {
        vec_text.push_front(move(word));
    }

    for (auto &text : vec_text)
    {
        if (key_words.count(text) != 0u)
        {
            result.word_frequences[text] += 1;
        }
    }
    return result;
}

using KeyWords = set<string, less<>>;

Stats ExploreKeyWords(const KeyWords &key_words, istream &input)
{
    list<future<Stats>> vec_stat;
    //vec_stat.reserve(50);
    Stats result;
    string text;
    while (input)
    {
        getline(input, text);
        vec_stat.push_front(async(GetStatsWords, cref(key_words), move(text)));
    }
    for (auto &vec : vec_stat)
    {
        result += vec.get();
    }
    return result;
}

int main()
{
    const KeyWords key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    for (const auto &[word, frequency] : ExploreKeyWords(key_words, ss).word_frequences)
    {
        cout << word << " " << frequency << endl;
    }

    return 0;
}