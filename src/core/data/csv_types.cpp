#include "core/data/csv_types.hpp"
#include <sstream>
namespace data
{
    std::optional<CsvRecord> CsvRecord::Parse(std::string_view line)
    {
        if (line.empty())
        {
            return std::nullopt;
        }
        while (!line.empty() && std::isspace(line.front()))
        {
            line.remove_prefix(1);
        }
        while (!line.empty() && std::isspace(line.back()))
        {
            line.remove_suffix(1);
        }
        if (line.empty())
        {
            return std::nullopt;
        }
        CsvRecord record;
        std::vector<std::string> fields;
        fields.reserve(5);
        size_t start = 0;
        size_t pos = 0;
        while (pos < line.size() && fields.size() < 5)
        {
            if (line[pos] == ',')
            {
                fields.emplace_back(line.substr(start, pos - start));
                start = pos + 1;
            }
            pos++;
        }
        if (start <= line.size())
        {
            fields.emplace_back(line.substr(start));
        }
        if (fields.size() != 5)
        {
            return std::nullopt;
        }
        record.timestamp = fields[0];
        record.product = fields[1];
        record.order_type = fields[2];
        record.price = fields[3];
        record.amount = fields[4];
        if (!record.IsValid())
        {
            return std::nullopt;
        }
        return record;
    }
    std::string CsvRecord::ToCsvLine() const
    {
        std::ostringstream oss;
        oss << timestamp << ',' << product << ',' << order_type << ',' << price << ',' << amount;
        return oss.str();
    }
    bool CsvRecord::IsValid() const
    {
        return !timestamp.empty() && !product.empty() && !order_type.empty() && !price.empty() &&
               !amount.empty();
    }
}