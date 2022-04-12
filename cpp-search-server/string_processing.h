#pragma once
#include <set>
#include <vector>
#include <string>

template <typename StringContainer>
std::set<std::string_view> MakeUniqueNonEmptyStrings(const StringContainer &strings)
{
    set<string, less<>> non_empty_strings;
    for (const auto& str : strings)
    {
        if (!str.empty())
        {
            non_empty_strings.insert(str);
        }
    }
    return non_empty_strings;
}

std::vector<std::string_view> SplitIntoWords(const std::string_view &text);
