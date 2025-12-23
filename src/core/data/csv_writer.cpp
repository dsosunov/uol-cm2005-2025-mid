#include "core/data/csv_writer.hpp"
#include <algorithm>
namespace data
{
    CsvWriter::CsvWriter(const std::filesystem::path &file_path, bool append, size_t buffer_size)
        : file_path_(file_path), append_mode_(append), buffer_size_(buffer_size)
    {
        buffer_.reserve(buffer_size_);
        if (file_path_.has_parent_path())
        {
            std::filesystem::create_directories(file_path_.parent_path());
        }
    }
    CsvWriter::~CsvWriter() noexcept
    {
        try
        {
            Flush();
        }
        catch (const std::exception &)
        {
        }
    }

    CsvWriter::CsvWriter(CsvWriter &&other) noexcept
        : file_path_(std::move(other.file_path_)),
          append_mode_(other.append_mode_),
          buffer_size_(other.buffer_size_),
          buffer_(std::move(other.buffer_)),
          has_flushed_(other.has_flushed_)
    {
        other.buffer_size_ = 0;
        other.has_flushed_ = false;
    }

    CsvWriter &CsvWriter::operator=(CsvWriter &&other) noexcept
    {
        if (this != &other)
        {
            try
            {
                Flush();
            }
            catch (const std::exception &)
            {
            }
            file_path_ = std::move(other.file_path_);
            append_mode_ = other.append_mode_;
            buffer_size_ = other.buffer_size_;
            buffer_ = std::move(other.buffer_);
            has_flushed_ = other.has_flushed_;
            other.buffer_size_ = 0;
            other.has_flushed_ = false;
        }
        return *this;
    }
    bool CsvWriter::Write(const CsvRecord &record)
    {
        if (!record.IsValid())
        {
            return false;
        }
        buffer_.push_back(record);
        if (buffer_size_ > 0 && buffer_.size() >= buffer_size_)
        {
            return FlushInternal();
        }
        return true;
    }
    bool CsvWriter::WriteAll(const std::vector<CsvRecord> &records)
    {
        for (const auto &record : records)
        {
            if (!Write(record))
            {
                return false;
            }
        }
        return true;
    }
    bool CsvWriter::Flush()
    {
        if (buffer_.empty())
        {
            return true;
        }
        return FlushInternal();
    }
    bool CsvWriter::IsOpen() const
    {
        std::ofstream test_file(file_path_, std::ios::app);
        return test_file.is_open();
    }
    const std::filesystem::path &CsvWriter::GetFilePath() const
    {
        return file_path_;
    }
    size_t CsvWriter::GetBufferSize() const
    {
        return buffer_.size();
    }
    void CsvWriter::ClearBuffer()
    {
        buffer_.clear();
    }
    bool CsvWriter::FlushInternal()
    {
        if (buffer_.empty())
        {
            return true;
        }
        std::ios::openmode mode = std::ios::out;
        if (append_mode_ || has_flushed_)
        {
            mode |= std::ios::app;
        }
        else
        {
            mode |= std::ios::trunc;
        }
        std::ofstream file(file_path_, mode);
        if (!file.is_open())
        {
            return false;
        }
        for (const auto &record : buffer_)
        {
            file << record.ToCsvLine() << '\n';
        }
        file.close();
        buffer_.clear();
        has_flushed_ = true;
        return true;
    }
}