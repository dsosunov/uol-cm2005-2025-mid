#pragma once
#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>
namespace data
{
struct CsvRecord
{
    std::vector<std::string> fields;
    bool IsValid() const
    {
        return !fields.empty();
    }
};
class CsvReader
{
  public:
    explicit CsvReader(const std::filesystem::path& file_path);
    ~CsvReader();
    CsvReader(const CsvReader&) = delete;
    CsvReader& operator=(const CsvReader&) = delete;
    CsvReader(CsvReader&&) noexcept = default;
    CsvReader& operator=(CsvReader&&) noexcept = default;
    bool IsOpen() const;
    const std::filesystem::path& GetFilePath() const;
    bool FileExists() const;
    template <typename Func> void ReadWithProcessor(Func processor) const
    {
        std::ifstream file(file_path_);
        if (!file.is_open())
        {
            return;
        }
        std::string line;
        while (std::getline(file, line))
        {
            auto record_opt = ParseRecord(line);
            if (record_opt.has_value())
            {
                processor(*record_opt);
            }
        }
    }
    template <typename Func> void ReadReverseWithProcessor(Func processor) const
    {
        if (!FileExists())
        {
            return;
        }

        std::ifstream file(file_path_);
        if (!file.is_open())
        {
            return;
        }

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line))
        {
            if (!line.empty())
            {
                lines.push_back(line);
            }
        }

        for (auto it = lines.rbegin(); it != lines.rend(); ++it)
        {
            auto record_opt = ParseRecord(*it);
            if (record_opt.has_value())
            {
                processor(*record_opt);
            }
        }
    }

  private:
    std::filesystem::path file_path_;
    static std::optional<CsvRecord> ParseRecord(std::string_view line);
    static std::string_view TrimWhitespace(std::string_view line);
    static size_t ParseQuotedField(std::string_view line, size_t pos, std::string& field);
    static size_t ParseUnquotedField(std::string_view line, size_t pos, std::string& field);
};
} // namespace data