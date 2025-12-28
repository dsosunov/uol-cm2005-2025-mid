#pragma once
#include "persistence/base_data_adapter.hpp"

#include <memory>
#include <optional>
#include <string_view>

namespace data
{
class CsvWriter;
}
namespace services
{
struct UserRecord;
}
namespace persistence
{
class UserDataAdapter : public BaseDataAdapter<services::UserRecord>
{
  public:
    UserDataAdapter(std::shared_ptr<data::CsvReader> reader,
                    std::shared_ptr<data::CsvWriter> writer);
    std::optional<services::UserRecord> FindByUsername(std::string_view username) const;
    bool ExistsByUsername(std::string_view username) const;
    bool Insert(services::UserRecord& user) const;
    bool Update(const services::UserRecord& user) const;

  protected:
    std::optional<services::UserRecord> TransformToEntity(
        const data::CsvRecord& record) const override;

  private:
    std::shared_ptr<data::CsvWriter> writer_;
    static data::CsvRecord TransformFromUserRecord(const services::UserRecord& user);
    std::vector<services::UserRecord> LoadAll() const;
    bool SaveAll(const std::vector<services::UserRecord>& users) const;
    int GetNextId() const;
};
} // namespace persistence
