#include "services/user_service.hpp"

#include "persistence/user_data_adapter.hpp"

#include <format>
#include <functional>
#include <random>

namespace services
{

UserService::UserService(std::shared_ptr<persistence::UserDataAdapter> adapter)
    : adapter_(std::move(adapter))
{
}

std::string UserService::GenerateUsername() const
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(0ULL, 9999999999ULL);

    for (int attempt = 0; attempt < 200; ++attempt)
    {
        std::string username = std::format("{:010}", dist(gen));
        if (!adapter_->ExistsByUsername(username))
        {
            return username;
        }
    }

    // Fallback (extremely unlikely): scan sequentially.
    for (uint64_t n = 0; n <= 9999999999ULL; ++n)
    {
        std::string username = std::format("{:010}", n);
        if (!adapter_->ExistsByUsername(username))
        {
            return username;
        }
    }

    return "0000000000";
}

utils::ServiceResult<User> UserService::RegisterUser(std::string_view username,
                                                     std::string_view full_name,
                                                     std::string_view email,
                                                     std::string_view password) const
{
    if (adapter_->ExistsByUsername(username))
    {
        return utils::ServiceResult<User>::Failure("Username already registered");
    }

    UserRecord new_record{0, std::string(username), std::string(full_name), std::string(email),
                          HashPassword(password)};

    if (!adapter_->Insert(new_record))
    {
        return utils::ServiceResult<User>::Failure("Failed to save user");
    }

    return utils::ServiceResult<User>::Success(ToUser(new_record), "Registration successful");
}

utils::ServiceResult<User> UserService::LoginUser(std::string_view username,
                                                  std::string_view password)
{
    size_t password_hash = HashPassword(password);
    auto user_record = adapter_->FindByUsername(username);

    if (!user_record)
    {
        return utils::ServiceResult<User>::Failure("User not found");
    }

    if (user_record->password_hash != password_hash)
    {
        return utils::ServiceResult<User>::Failure("Invalid password");
    }

    current_user_ = ToUser(*user_record);

    return utils::ServiceResult<User>::Success(*current_user_, "Login successful");
}

utils::ServiceResult<void> UserService::ResetPassword(std::string_view username,
                                                      std::string_view new_password) const
{
    auto user_record = adapter_->FindByUsername(username);

    if (!user_record)
    {
        return utils::ServiceResult<void>::Failure("User not found");
    }

    user_record->password_hash = HashPassword(new_password);

    if (!adapter_->Update(*user_record))
    {
        return utils::ServiceResult<void>::Failure("Failed to save password");
    }

    return utils::ServiceResult<void>::Success("Password reset successful");
}

utils::ServiceResult<User> UserService::GetCurrentUser() const
{
    if (current_user_.has_value())
    {
        return {true, "Current user retrieved successfully", *current_user_};
    }
    return utils::ServiceResult<User>::Failure("No user logged in");
}

utils::ServiceResult<void> UserService::Logout()
{
    current_user_ = std::nullopt;
    return {true, "Logout successful"};
}

utils::ServiceResult<bool> UserService::IsLoggedIn() const
{
    return {true, "Login status retrieved successfully", current_user_.has_value()};
}

size_t UserService::HashPassword(std::string_view password)
{
    return std::hash<std::string_view>{}(password);
}

User UserService::ToUser(const UserRecord& record)
{
    return User{record.id, record.username, record.full_name, record.email};
}

} // namespace services