#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

bool isPalindrome(string s)
{
    int var = 0;
    string prob, text;
    for (auto i = begin(s); i < end(s); i++) {
        if ( !(*i >= '\0' && *i < '0' || *i > '9' && *i < 'A' || *i > 'Z' && *i < 'a' || *i > 'z')) {
            *i = tolower(*i);
            text.push_back(*i);
        }
    }
    for (int i = 0; i < text.size(); i++)
    {
        if (text[i] != ' ')
        {
            prob.push_back(text[i]);
        }
    }
    if (prob.empty())
    {
        return true;
    }
    else
    {
        int size_prob = prob.size();
        for (int i = 0; i < (size_prob / 2); i++)
        {
            if (prob[i] == prob[size_prob - i - 1])
            {
                var++;
            }
            else
            {
                break;
            }
        }
        if (var == size_prob / 2)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

int main()
{
    string text = "ab _a";

    if (isPalindrome(text))
    {
        cout << "palindrome"s << endl;
    }
    else
    {
        cout << "not a palindrome"s << endl;
    }
}