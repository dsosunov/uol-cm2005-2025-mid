#pragma once

#include <set>
#include <string>
#include <string_view>

namespace utils
{
struct StringUtils
{
    static std::string Join(const std::set<std::string, std::less<>>& items,
                            std::string_view delimiter)
    {
        std::string out;
        bool first = true;
        for (const auto& item : items)
        {
            if (!first)
            {
                out += delimiter;
            }
            first = false;
            out += item;
        }
        return out;
    }
};
} // namespace utils
