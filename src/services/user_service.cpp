#include "services/user_service.hpp"
#include "persistence/user_data_adapter.hpp"
#include <functional>
#include <format>
namespace services
{
    UserService::UserService(std::shared_ptr<persistence::UserDataAdapter> adapter)
        : adapter_(std::move(adapter))
    {
    }
    utils::ServiceResult<User> UserService::RegisterUser(std::string_view full_name,
                                                         std::string_view email,
                                                         std::string_view password)
    {
        if (adapter_->ExistsByEmail(email))
        {
            return utils::ServiceResult<User>::Failure("Email already registered");
        }
        std::string username(email.substr(0, email.find('@')));
        std::string original_username = username;
        int suffix = 1;
        while (adapter_->ExistsByUsername(username))
        {
            username = std::format("{}{}", original_username, suffix);
            ++suffix;
        }
        UserRecord new_record{0, std::string(full_name), std::string(email),
                              username, HashPassword(password)};
        if (!adapter_->Insert(new_record))
        {
            return utils::ServiceResult<User>::Failure("Failed to save user");
        }
        return utils::ServiceResult<User>::Success(ToUser(new_record), "Registration successful");
    }
    utils::ServiceResult<User> UserService::LoginUser(std::string_view username, std::string_view password)
    {
        size_t password_hash = HashPassword(password);
        auto user_record = adapter_->FindByUsername(username);
        if (!user_record)
        {
            user_record = adapter_->FindByEmail(username);
        }
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
    utils::ServiceResult<void> UserService::ResetPassword(std::string_view email_or_username,
                                                          std::string_view new_password)
    {
        auto user_record = adapter_->FindByUsername(email_or_username);
        if (!user_record)
        {
            user_record = adapter_->FindByEmail(email_or_username);
        }
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
    std::optional<User> UserService::GetCurrentUser() const
    {
        return current_user_;
    }
    void UserService::Logout()
    {
        current_user_ = std::nullopt;
    }
    bool UserService::IsLoggedIn() const
    {
        return current_user_.has_value();
    }
    size_t UserService::HashPassword(std::string_view password)
    {
        return std::hash<std::string_view>{}(password);
    }
    User UserService::ToUser(const UserRecord &record)
    {
        return User{record.id, record.full_name, record.email, record.username};
    }
}