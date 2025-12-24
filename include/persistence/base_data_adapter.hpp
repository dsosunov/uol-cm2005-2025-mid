#pragma once
#include "core/data/csv_reader.hpp"
#include "core/data/csv_writer.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <vector>

namespace persistence
{
template <typename TEntity> class BaseDataAdapter
{
  public:
    explicit BaseDataAdapter(std::shared_ptr<data::CsvReader> reader) : reader_(std::move(reader))
    {
    }
    virtual ~BaseDataAdapter() = default;
    bool IsValid() const
    {
        return reader_ && reader_->FileExists();
    }
    void ReadWithProcessor(std::function<void(const TEntity&)> processor) const
    {
        if (!IsValid())
        {
            return;
        }
        reader_->ReadWithProcessor([this, &processor](const data::CsvRecord& csv_record) {
            auto entity = TransformToEntity(csv_record);
            if (entity.has_value())
            {
                processor(*entity);
            }
        });
    }
    template <typename TPredicate>
    void ReadWithProcessor(TPredicate predicate,
                           std::function<void(const TEntity&)> processor) const
    {
        if (!IsValid())
        {
            return;
        }
        reader_->ReadWithProcessor(
            [this, &predicate, &processor](const data::CsvRecord& csv_record) {
                auto entity = TransformToEntity(csv_record);
                if (entity.has_value() && predicate(*entity))
                {
                    processor(*entity);
                }
            });
    }
    static bool WriteAll(data::CsvWriter& writer, const std::vector<TEntity>& entities,
                         std::function<data::CsvRecord(const TEntity&)> transformer)
    {
        for (const auto& entity : entities)
        {
            data::CsvRecord record = transformer(entity);
            if (!writer.Write(record))
            {
                return false;
            }
        }
        return writer.Flush();
    }

  protected:
    virtual std::optional<TEntity> TransformToEntity(const data::CsvRecord& record) const = 0;

  private:
    std::shared_ptr<data::CsvReader> reader_;
};
} // namespace persistence
