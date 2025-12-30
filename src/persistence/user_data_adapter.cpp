#include "persistence/user_data_adapter.hpp"

#include "core/data/csv_writer.hpp"
#include "services/user_models.hpp"

#include <cstdint>
#include <sstream>

namespace persistence
{

UserDataAdapter::UserDataAdapter(std::shared_ptr<data::CsvReader> reader,
                                 std::shared_ptr<data::CsvWriter> writer)
    : BaseDataAdapter<services::UserRecord>(reader), writer_(std::move(writer))
{
}

std::optional<services::UserRecord> UserDataAdapter::FindByUsername(std::string_view username) const
{
    std::optional<services::UserRecord> result;

    ReadWithProcessor(
        [username](const services::UserRecord& user) { return user.username == username; },
        [&result](const services::UserRecord& user) { result = user; });

    return result;
}

bool UserDataAdapter::ExistsByUsername(std::string_view username) const
{
    bool exists = false;

    ReadWithProcessor(
        [username](const services::UserRecord& user) { return user.username == username; },
        [&exists](const services::UserRecord&) { exists = true; });

    return exists;
}

std::optional<services::UserRecord> UserDataAdapter::FindByEmail(std::string_view email) const
{
    std::optional<services::UserRecord> result;

    ReadWithProcessor([email](const services::UserRecord& user) { return user.email == email; },
                      [&result](const services::UserRecord& user) { result = user; });

    return result;
}

bool UserDataAdapter::ExistsByEmail(std::string_view email) const
{
    bool exists = false;

    ReadWithProcessor([email](const services::UserRecord& user) { return user.email == email; },
                      [&exists](const services::UserRecord&) { exists = true; });

    return exists;
}

bool UserDataAdapter::Insert(services::UserRecord& user) const
{
    user.id = GetNextId();

    auto users = LoadAll();
    users.push_back(user);

    return SaveAll(users);
}

bool UserDataAdapter::Update(const services::UserRecord& user) const
{
    auto users = LoadAll();
    bool found = false;

    for (auto& u : users)
    {
        if (u.id == user.id)
        {
            u = user;
            found = true;
            break;
        }
    }

    return found && SaveAll(users);
}

std::optional<services::UserRecord> UserDataAdapter::TransformToEntity(
    const data::CsvRecord& record) const
{
    if (record.fields.size() < 5)
    {
        return std::nullopt;
    }

    services::UserRecord user;

    try
    {
        user.id = std::stoi(record.fields[0]);
        user.username = record.fields[1];
        user.full_name = record.fields[2];
        user.email = record.fields[3];
        user.password_hash = static_cast<std::uint64_t>(std::stoull(record.fields[4]));
    }
    catch (const std::invalid_argument&)
    {
        return std::nullopt;
    }
    catch (const std::out_of_range&)
    {
        return std::nullopt;
    }

    return user;
}

data::CsvRecord UserDataAdapter::TransformFromUserRecord(const services::UserRecord& user)
{
    data::CsvRecord record;
    record.fields.reserve(5);

    record.fields.push_back(std::to_string(user.id));
    record.fields.push_back(user.username);
    record.fields.push_back(user.full_name);
    record.fields.push_back(user.email);
    record.fields.push_back(std::to_string(static_cast<unsigned long long>(user.password_hash)));

    return record;
}

std::vector<services::UserRecord> UserDataAdapter::LoadAll() const
{
    std::vector<services::UserRecord> users;

    if (IsValid())
    {
        ReadWithProcessor([&users](const services::UserRecord& user) { users.push_back(user); });
    }

    return users;
}

bool UserDataAdapter::SaveAll(const std::vector<services::UserRecord>& users) const
{
    if (!writer_)
    {
        return false;
    }

    writer_->ClearBuffer();

    return BaseDataAdapter<services::UserRecord>::WriteAll(*writer_, users,
                                                           TransformFromUserRecord);
}

int UserDataAdapter::GetNextId() const
{
    int max_id = 0;

    ReadWithProcessor([&max_id](const services::UserRecord& user) {
        if (user.id > max_id)
        {
            max_id = user.id;
        }
    });

    return max_id + 1;
}

} // namespace persistence
