#include <iostream>
#include <string>

using namespace std;

bool IsPalindrome(const string &text)
{
    int size_text = text.size(), var = 0;
    string prob;
    for (int i = 0; i < size_text; i++)
    {
        if (text[i] != ' ')
        {
            prob.push_back(text[i]);
        }
    }
    if (prob.empty())
    {
        return false;
    }
    else
    {
        int size_prob = prob.size();
        for (int i = 0; i < (size_prob / 2); i++)
        {
            if (prob[i] == prob[size_text - i - 1])
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
    string text = "never odd or even";

    if (IsPalindrome(text))
    {
        cout << "palindrome"s << endl;
    }
    else
    {
        cout << "not a palindrome"s << endl;
    }
}