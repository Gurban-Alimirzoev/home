#pragma once
#include <set>
#include <vector>
#include <string>

template <typename StringContainer>
const std::set<std::string, std::less<>> MakeUniqueNonEmptyStrings(const StringContainer &strings)
{
    const std::set<std::string, std::less<>> non_empty_strings;
    for (const std::string& str : strings)
    {
        if (!str.empty())
        {
            non_empty_strings.insert(str);
        }
    }
    return non_empty_strings;
}

std::vector<std::string_view> SplitIntoWords(const std::string_view &text);
