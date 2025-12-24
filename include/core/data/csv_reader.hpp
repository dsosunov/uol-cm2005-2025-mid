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

        std::ifstream file(file_path_, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            return;
        }

        std::streampos file_size = file.tellg();
        if (file_size == 0)
        {
            return;
        }

        std::vector<std::string> lines;
        std::streamoff pos = static_cast<std::streamoff>(file_size);

        while (pos > 0)
        {
            --pos;
            file.seekg(pos);

            char ch;
            file.get(ch);

            if (ch == '\n' || pos == 0)
            {
                std::streamoff line_start = (ch == '\n' && pos > 0) ? pos + 1 : pos;
                file.seekg(line_start);

                std::string current_line;
                std::getline(file, current_line);

                if (!current_line.empty() || ch == '\n')
                {
                    lines.push_back(std::move(current_line));
                }
            }
        }

        // Process all lines in reverse order (last line first)
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