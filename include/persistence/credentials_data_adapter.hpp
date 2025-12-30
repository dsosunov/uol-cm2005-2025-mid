#pragma once

#include "persistence/base_data_adapter.hpp"

#include <memory>
#include <optional>

namespace data
{
class CsvWriter;
}

namespace services
{
struct CredentialRecord;
}

namespace persistence
{

class CredentialsDataAdapter : public BaseDataAdapter<services::CredentialRecord>
{
  public:
    CredentialsDataAdapter(std::shared_ptr<data::CsvReader> reader,
                           std::shared_ptr<data::CsvWriter> writer);

    std::optional<services::CredentialRecord> FindByUserId(int user_id) const;
    bool ExistsByUserId(int user_id) const;

    bool Upsert(const services::CredentialRecord& credentials) const;

  protected:
    std::optional<services::CredentialRecord> TransformToEntity(
        const data::CsvRecord& record) const override;

  private:
    std::shared_ptr<data::CsvWriter> writer_;

    static data::CsvRecord TransformFromCredentials(const services::CredentialRecord& credentials);
    std::vector<services::CredentialRecord> LoadAll() const;
    bool SaveAll(const std::vector<services::CredentialRecord>& credentials) const;
};

} // namespace persistence
