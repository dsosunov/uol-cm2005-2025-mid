#pragma once
#include "core/utils/service_result.hpp"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace services
{
class AuthenticationService;
}

namespace persistence
{
class UserDataAdapter;
}
namespace services
{
struct User
{
    int id;
    std::string username;
    std::string full_name;
    std::string email;
};
struct UserRecord
{
    int id;
    std::string username;
    std::string full_name;
    std::string email;
    std::uint64_t password_hash;
};
class UserService
{
  public:
    UserService(std::shared_ptr<persistence::UserDataAdapter> adapter,
                std::shared_ptr<services::AuthenticationService> auth_service);
    ~UserService() = default;
    std::string GenerateUsername() const;
    utils::ServiceResult<User> RegisterUser(std::string_view username, std::string_view full_name,
                                            std::string_view email,
                                            std::string_view password) const;
    utils::ServiceResult<std::string> RemindUsername(std::string_view email) const;
    utils::ServiceResult<User> LoginUser(std::string_view username, std::string_view password);
    utils::ServiceResult<void> ResetPassword(std::string_view username,
                                             std::string_view new_password) const;
    utils::ServiceResult<User> GetCurrentUser() const;
    utils::ServiceResult<void> Logout();
    utils::ServiceResult<bool> IsLoggedIn() const;

  private:
    std::shared_ptr<persistence::UserDataAdapter> adapter_;
    std::shared_ptr<services::AuthenticationService> auth_service_;
    static std::uint64_t HashPassword(std::string_view password);
    static User ToUser(const UserRecord& record);
};
} // namespace services