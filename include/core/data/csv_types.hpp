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
        std::string timestamp;
        std::string product;
        std::string order_type;
        std::string price;
        std::string amount;
        static std::optional<CsvRecord> Parse(std::string_view line);
        std::string ToCsvLine() const;
        bool IsValid() const;
    };
    using RecordFilter = std::function<bool(const CsvRecord &)>;
    template <typename T>
    using RecordTransform = std::function<std::optional<T>(const CsvRecord &)>;
}