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
    std::vector<CsvRecord> CsvReader::ReadAll(RecordFilter filter) const
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
            auto record_opt = CsvRecord::Parse(line);
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
    size_t CsvReader::Count(RecordFilter filter) const
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
            auto record_opt = CsvRecord::Parse(line);
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
}