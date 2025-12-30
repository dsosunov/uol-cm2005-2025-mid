#pragma once
#include "core/data/csv_reader.hpp"

#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

namespace data
{
class CsvWriter
{
  public:
    explicit CsvWriter(const std::filesystem::path& file_path, bool append = false,
                       size_t buffer_size = 100);
    ~CsvWriter() noexcept;
    CsvWriter(const CsvWriter&) = delete;
    CsvWriter& operator=(const CsvWriter&) = delete;
    CsvWriter(CsvWriter&& other) noexcept;
    CsvWriter& operator=(CsvWriter&& other) noexcept;
    bool Write(const CsvRecord& record);
    bool WriteAll(const std::vector<CsvRecord>& records);
    bool Flush();
    bool IsOpen() const;
    const std::filesystem::path& GetFilePath() const;
    size_t GetBufferSize() const;
    void ClearBuffer();

  private:
    std::filesystem::path file_path_;
    bool append_mode_;
    size_t buffer_size_;
    std::vector<CsvRecord> buffer_;
    bool FlushInternal();
    static std::string FormatRecord(const CsvRecord& record);
    static std::string EscapeField(const std::string& field);
};
} // namespace data