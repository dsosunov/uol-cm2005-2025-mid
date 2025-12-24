#pragma once
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
namespace data
{
    struct CsvRecord
    {
        std::vector<std::string> fields;
        bool IsValid() const;
    };

}