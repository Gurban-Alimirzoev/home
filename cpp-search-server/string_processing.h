#pragma once
#include <set>
#include <vector>
#include <string>
#include <algorithm>

template <typename StringContainer>
std::set<std::string, std::less<>> MakeUniqueNonEmptyStrings(const StringContainer& strings)
{
	std::set<std::string, std::less<>> non_empty_strings;

	for (const auto& str : strings)
	{
		if (!str.empty())
		{
			non_empty_strings.insert(std::string(str));
		}
	}
	return non_empty_strings;
}

std::vector<std::string> SplitIntoWords(const std::string_view text);
