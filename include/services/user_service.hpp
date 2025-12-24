#pragma once
#include "core/utils/service_result.hpp"

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace persistence
{
class UserDataAdapter;
}
namespace services
{
struct User
{
    int id;
    std::string full_name;
    std::string email;
};
struct UserRecord
{
    int id;
    std::string full_name;
    std::string email;
    size_t password_hash;
};
class UserService
{
  public:
    explicit UserService(std::shared_ptr<persistence::UserDataAdapter> adapter);
    ~UserService() = default;
    utils::ServiceResult<User> RegisterUser(std::string_view full_name, std::string_view email,
                                            std::string_view password) const;
    utils::ServiceResult<User> LoginUser(std::string_view email, std::string_view password);
    utils::ServiceResult<void> ResetPassword(std::string_view email,
                                             std::string_view new_password) const;
    std::optional<User> GetCurrentUser() const;
    void Logout();
    bool IsLoggedIn() const;

  private:
    std::shared_ptr<persistence::UserDataAdapter> adapter_;
    std::optional<User> current_user_;
    static size_t HashPassword(std::string_view password);
    static User ToUser(const UserRecord& record);
};
} // namespace services