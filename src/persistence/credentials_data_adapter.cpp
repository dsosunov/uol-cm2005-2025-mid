#include "persistence/credentials_data_adapter.hpp"

#include "core/data/csv_writer.hpp"
#include "services/credentials_models.hpp"

#include <sstream>

namespace persistence
{

CredentialsDataAdapter::CredentialsDataAdapter(std::shared_ptr<data::CsvReader> reader,
                                               std::shared_ptr<data::CsvWriter> writer)
    : BaseDataAdapter<services::CredentialRecord>(std::move(reader)), writer_(std::move(writer))
{
}

std::optional<services::CredentialRecord> CredentialsDataAdapter::FindByUserId(int user_id) const
{
    std::optional<services::CredentialRecord> result;

    ReadWithProcessor(
        [user_id](const services::CredentialRecord& c) { return c.user_id == user_id; },
        [&result](const services::CredentialRecord& c) { result = c; });

    return result;
}

bool CredentialsDataAdapter::ExistsByUserId(int user_id) const
{
    bool exists = false;

    ReadWithProcessor(
        [user_id](const services::CredentialRecord& c) { return c.user_id == user_id; },
        [&exists](const services::CredentialRecord&) { exists = true; });

    return exists;
}

bool CredentialsDataAdapter::Upsert(const services::CredentialRecord& credentials) const
{
    auto all = LoadAll();
    bool found = false;

    for (auto& c : all)
    {
        if (c.user_id == credentials.user_id)
        {
            c = credentials;
            found = true;
            break;
        }
    }

    if (!found)
    {
        all.push_back(credentials);
    }

    return SaveAll(all);
}

std::optional<services::CredentialRecord> CredentialsDataAdapter::TransformToEntity(
    const data::CsvRecord& record) const
{
    if (record.fields.size() < 2)
    {
        return std::nullopt;
    }

    services::CredentialRecord credentials;

    try
    {
        credentials.user_id = std::stoi(record.fields[0]);
        credentials.password_hash = std::stoull(record.fields[1]);
    }
    catch (const std::invalid_argument&)
    {
        return std::nullopt;
    }
    catch (const std::out_of_range&)
    {
        return std::nullopt;
    }

    return credentials;
}

data::CsvRecord CredentialsDataAdapter::TransformFromCredentials(
    const services::CredentialRecord& credentials)
{
    data::CsvRecord record;
    record.fields.reserve(2);

    record.fields.push_back(std::to_string(credentials.user_id));
    record.fields.push_back(std::to_string(credentials.password_hash));

    return record;
}

std::vector<services::CredentialRecord> CredentialsDataAdapter::LoadAll() const
{
    std::vector<services::CredentialRecord> credentials;

    if (IsValid())
    {
        ReadWithProcessor(
            [&credentials](const services::CredentialRecord& c) { credentials.push_back(c); });
    }

    return credentials;
}

bool CredentialsDataAdapter::SaveAll(
    const std::vector<services::CredentialRecord>& credentials) const
{
    if (!writer_)
    {
        return false;
    }

    writer_->ClearBuffer();

    return BaseDataAdapter<services::CredentialRecord>::WriteAll(*writer_, credentials,
                                                                 TransformFromCredentials);
}

} // namespace persistence
