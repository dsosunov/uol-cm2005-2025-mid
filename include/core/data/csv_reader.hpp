#pragma once
#include "core/data/csv_types.hpp"
#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <string>
#include <vector>
namespace data
{
    class CsvReader
    {
    public:
        explicit CsvReader(const std::filesystem::path &file_path);
        ~CsvReader();
        CsvReader(const CsvReader &) = delete;
        CsvReader &operator=(const CsvReader &) = delete;
        CsvReader(CsvReader &&) noexcept = default;
        CsvReader &operator=(CsvReader &&) noexcept = default;
        bool IsOpen() const;
        const std::filesystem::path &GetFilePath() const;
        std::vector<CsvRecord> ReadAll(const RecordFilter &filter = nullptr) const;
        template <typename T>
        std::vector<T> ReadAllTransformed(RecordTransform<T> transform,
                                          const RecordFilter &filter = nullptr) const
        {
            std::vector<T> results;
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
                auto transformed = transform(record);
                if (transformed.has_value())
                {
                    results.push_back(std::move(*transformed));
                }
            }
            return results;
        }
        template <typename Func>
        void ReadWithProcessor(Func processor) const
        {
            std::ifstream file(file_path_);
            if (!file.is_open())
            {
                return;
            }
            std::string line;
            while (std::getline(file, line))
            {
                auto record_opt = CsvRecord::Parse(line);
                if (record_opt.has_value())
                {
                    processor(*record_opt);
                }
            }
        }
        size_t Count(const RecordFilter &filter = nullptr) const;
        bool FileExists() const;

    private:
        std::filesystem::path file_path_;
    };
}