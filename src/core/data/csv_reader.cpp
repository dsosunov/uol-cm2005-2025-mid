#include "core/data/csv_reader.hpp"
namespace data
{

CsvReader::CsvReader(const std::filesystem::path& file_path) : file_path_(file_path)
{
}

CsvReader::~CsvReader() = default;

bool CsvReader::IsOpen() const
{
    std::ifstream test_file(file_path_);

    return test_file.is_open();
}

const std::filesystem::path& CsvReader::GetFilePath() const
{
    return file_path_;
}

bool CsvReader::FileExists() const
{
    return std::filesystem::exists(file_path_) && std::filesystem::is_regular_file(file_path_);
}

std::string_view CsvReader::TrimWhitespace(std::string_view line)
{
    while (!line.empty() && std::isspace(line.front()))
    {
        line.remove_prefix(1);
    }

    while (!line.empty() && std::isspace(line.back()))
    {
        line.remove_suffix(1);
    }
    return line;
}

size_t CsvReader::ParseQuotedField(std::string_view line, size_t pos, std::string& field)
{
    pos++; // Skip opening quote

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

    return pos;
}

size_t CsvReader::ParseUnquotedField(std::string_view line, size_t pos, std::string& field)
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

    return pos;
}

std::optional<CsvRecord> CsvReader::ParseRecord(std::string_view line)
{
    if (line.empty())
    {
        return std::nullopt;
    }

    line = TrimWhitespace(line);
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
                record.fields.emplace_back("");
            }
            break;
        }

        std::string field;
        if (line[pos] == '"')
        {
            pos = ParseQuotedField(line, pos, field);
        }
        else
        {
            pos = ParseUnquotedField(line, pos, field);
        }

        record.fields.push_back(field);
    }

    if (record.fields.empty())
    {
        return std::nullopt;
    }

    return record;
}

} // namespace data