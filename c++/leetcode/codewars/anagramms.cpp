#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>

using std::cin;
using std::cout;

bool Annagramm(std::string f_word, std::string s_word)
{
	if (f_word.size() != s_word.size())
	{
		return false;
	}

	std::map<char, int> f_w_count;
	std::map<char, int> s_w_count;
	for (char c : f_word)
	{
		f_w_count[c]++; 
	}
	for (char c : s_word)
	{
		s_w_count[c]++; 
	}
	return f_w_count == s_w_count;

}

int main()
{
	std::string f_word;
	std::string s_word;
	cin >> f_word >> s_word;
	cout << Annagramm(f_word, s_word);
}