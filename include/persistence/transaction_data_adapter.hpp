#pragma once
#include "persistence/base_data_adapter.hpp"

#include <functional>
#include <memory>

namespace data
{
class CsvWriter;
}
namespace services
{
struct WalletTransaction;
}
namespace persistence
{
class TransactionDataAdapter : public BaseDataAdapter<services::WalletTransaction>
{
  public:
    TransactionDataAdapter(std::shared_ptr<data::CsvReader> reader,
                           std::shared_ptr<data::CsvWriter> writer);
    bool Add(const services::WalletTransaction& transaction);

  protected:
    std::optional<services::WalletTransaction> TransformToEntity(
        const data::CsvRecord& record) const override;

  private:
    std::shared_ptr<data::CsvWriter> writer_;
    static std::string CleanNumericField(const std::string& field);
    static data::CsvRecord TransformFromTransaction(const services::WalletTransaction& txn);
};
} // namespace persistence