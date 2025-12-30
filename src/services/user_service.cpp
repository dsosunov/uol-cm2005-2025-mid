#include "services/user_service.hpp"

#include "persistence/user_data_adapter.hpp"
#include "services/authentication_service.hpp"
#include "services/credentials_service.hpp"

#include <cstdint>
#include <functional>
#include <stdexcept>

namespace services
{

User UserService::ToUser(const UserRecord& record)
{
    return User{record.id, record.username, record.full_name, record.email};
}

UserService::UserService(std::shared_ptr<persistence::UserDataAdapter> adapter,
                         std::shared_ptr<services::AuthenticationService> auth_service,
                         std::shared_ptr<services::CredentialsService> credentials_service)
    : adapter_(std::move(adapter)), auth_service_(std::move(auth_service)),
      credentials_service_(std::move(credentials_service))
{
    if (!adapter_)
    {
        throw std::invalid_argument("UserDataAdapter is required");
    }
    if (!auth_service_)
    {
        throw std::invalid_argument("AuthenticationService is required");
    }
    if (!credentials_service_)
    {
        throw std::invalid_argument("CredentialsService is required");
    }
}

utils::ServiceResult<std::string> UserService::RemindUsername(std::string_view email) const
{
    auto user_record = adapter_->FindByEmail(email);

    if (!user_record)
    {
        return utils::ServiceResult<std::string>::Failure("User not found");
    }

    return utils::ServiceResult<std::string>::Success(user_record->username, "Username found");
}

utils::ServiceResult<User> UserService::LoginUser(std::string_view username,
                                                  std::string_view password)
{
    auto user_record = adapter_->FindByUsername(username);

    if (!user_record)
    {
        return utils::ServiceResult<User>::Failure("User not found");
    }

    if (auto pw = credentials_service_->ValidatePassword(user_record->id, password); !pw.success)
    {
        return utils::ServiceResult<User>::Failure(pw.message);
    }

    auto user = ToUser(*user_record);
    auth_service_->SetCurrentUser(
        services::AuthenticatedUser{user.id, user.username, user.full_name, user.email});

    return utils::ServiceResult<User>::Success(user, "Login successful");
}

utils::ServiceResult<void> UserService::ResetPassword(std::string_view username,
                                                      std::string_view new_password) const
{
    auto user_record = adapter_->FindByUsername(username);

    if (!user_record)
    {
        return utils::ServiceResult<void>::Failure("User not found");
    }

    if (auto saved = credentials_service_->SetPassword(user_record->id, new_password);
        !saved.success)
    {
        return saved;
    }

    return utils::ServiceResult<void>::Success("Password reset successful");
}

utils::ServiceResult<User> UserService::GetCurrentUser() const
{
    auto user_result = auth_service_->GetCurrentUser();
    const auto& au = user_result.data.value();
    return utils::ServiceResult<User>::Success(User{au.id, au.username, au.full_name, au.email},
                                               "Current user retrieved successfully");
}

utils::ServiceResult<void> UserService::Logout()
{
    auth_service_->Logout();
    return {true, "Logout successful"};
}

utils::ServiceResult<bool> UserService::IsLoggedIn() const
{
    return {true, "Login status retrieved successfully", auth_service_->IsAuthenticated()};
}

utils::ServiceResult<void> UserService::ValidateUserId(int user_id) const
{
    if (user_id <= 0)
    {
        return utils::ServiceResult<void>::Failure("Invalid user id");
    }

    bool found = false;
    adapter_->ReadWithProcessor(
        [user_id](const UserRecord& record) { return record.id == user_id; },
        [&found](const UserRecord& record) {
            (void)record;
            found = true;
        });

    if (!found)
    {
        return utils::ServiceResult<void>::Failure("User not found");
    }

    return utils::ServiceResult<void>::Success("User id is valid");
}

} // namespace services