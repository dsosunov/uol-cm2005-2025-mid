#pragma once
#include "persistence/base_data_adapter.hpp"

#include <functional>
#include <memory>

namespace services
{
struct Transaction;
}
namespace persistence
{
class TransactionDataAdapter : public BaseDataAdapter<services::Transaction>
{
  public:
    explicit TransactionDataAdapter(std::shared_ptr<data::CsvReader> reader);
    static bool WriteAll(data::CsvWriter& writer,
                         const std::vector<services::Transaction>& transactions);

  protected:
    std::optional<services::Transaction> TransformToEntity(
        const data::CsvRecord& record) const override;

  private:
    static std::string CleanNumericField(const std::string& field);
    static data::CsvRecord TransformFromTransaction(const services::Transaction& txn);
};
} // namespace persistence