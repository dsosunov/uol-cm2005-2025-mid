#pragma once
#include "core/utils/service_result.hpp"
#include "services/user_models.hpp"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace services
{
class AuthenticationService;
class CredentialsService;
} // namespace services

namespace persistence
{
class UserDataAdapter;
}
namespace services
{
class UserService
{
  public:
    UserService(std::shared_ptr<persistence::UserDataAdapter> adapter,
                std::shared_ptr<services::AuthenticationService> auth_service,
                std::shared_ptr<services::CredentialsService> credentials_service);
    ~UserService() = default;
    utils::ServiceResult<std::string> RemindUsername(std::string_view email) const;
    utils::ServiceResult<User> LoginUser(std::string_view username, std::string_view password);
    utils::ServiceResult<void> ResetPassword(std::string_view username,
                                             std::string_view new_password) const;
    utils::ServiceResult<User> GetCurrentUser() const;
    utils::ServiceResult<void> Logout();
    utils::ServiceResult<bool> IsLoggedIn() const;
    utils::ServiceResult<void> ValidateUserId(int user_id) const;

  private:
    static User ToUser(const UserRecord& record);
    std::shared_ptr<persistence::UserDataAdapter> adapter_;
    std::shared_ptr<services::AuthenticationService> auth_service_;
    std::shared_ptr<services::CredentialsService> credentials_service_;
};
} // namespace services