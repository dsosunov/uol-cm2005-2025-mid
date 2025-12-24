#include "core/data/csv_reader.hpp"
namespace data
{
    CsvReader::CsvReader(const std::filesystem::path &file_path) : file_path_(file_path)
    {
    }
    CsvReader::~CsvReader() = default;
    bool CsvReader::IsOpen() const
    {
        std::ifstream test_file(file_path_);
        return test_file.is_open();
    }
    const std::filesystem::path &CsvReader::GetFilePath() const
    {
        return file_path_;
    }
    std::vector<CsvRecord> CsvReader::ReadAll(const RecordFilter &filter) const
    {
        std::vector<CsvRecord> results;
        results.reserve(100);
        std::ifstream file(file_path_);
        if (!file.is_open())
        {
            return results;
        }
        std::string line;
        while (std::getline(file, line))
        {
            auto record_opt = ParseRecord(line);
            if (!record_opt.has_value())
            {
                continue;
            }
            const auto &record = *record_opt;
            if (filter != nullptr && !filter(record))
            {
                continue;
            }
            results.push_back(record);
        }
        return results;
    }
    size_t CsvReader::Count(const RecordFilter &filter) const
    {
        size_t count = 0;
        std::ifstream file(file_path_);
        if (!file.is_open())
        {
            return 0;
        }
        std::string line;
        while (std::getline(file, line))
        {
            auto record_opt = ParseRecord(line);
            if (!record_opt.has_value())
            {
                continue;
            }
            if (filter == nullptr || filter(*record_opt))
            {
                count++;
            }
        }
        return count;
    }
    bool CsvReader::FileExists() const
    {
        return std::filesystem::exists(file_path_) && std::filesystem::is_regular_file(file_path_);
    }
    std::optional<CsvRecord> CsvReader::ParseRecord(std::string_view line)
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
        size_t pos = 0;
        while (pos <= line.size())
        {
            if (pos == line.size())
            {
                if (!record.fields.empty() || pos > 0)
                {
                    record.fields.push_back("");
                }
                break;
            }
            std::string field;
            if (line[pos] == '"')
            {
                pos++;
                while (pos < line.size())
                {
                    if (line[pos] == '"')
                    {
                        if (pos + 1 < line.size() && line[pos + 1] == '"')
                        {
                            field += '"';
                            pos += 2;
                        }
                        else
                        {
                            pos++;
                            break;
                        }
                    }
                    else
                    {
                        field += line[pos];
                        pos++;
                    }
                }
                while (pos < line.size() && line[pos] != ',')
                {
                    pos++;
                }
                if (pos < line.size() && line[pos] == ',')
                {
                    pos++;
                }
            }
            else
            {
                size_t start = pos;
                while (pos < line.size() && line[pos] != ',')
                {
                    pos++;
                }
                field = std::string(line.substr(start, pos - start));
                if (pos < line.size() && line[pos] == ',')
                {
                    pos++;
                }
            }
            record.fields.push_back(field);
        }
        if (record.fields.empty())
        {
            return std::nullopt;
        }
        return record;
    }
}