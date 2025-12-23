#pragma once
#include "core/utils/service_result.hpp"
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
namespace services
{
    struct User
    {
        int id;
        std::string full_name;
        std::string email;
        std::string username;
    };
    class UserService
    {
    public:
        UserService();
        ~UserService() = default;
        utils::ServiceResult<User> RegisterUser(std::string_view full_name, std::string_view email,
                                                std::string_view password);
        utils::ServiceResult<User> LoginUser(std::string_view username, std::string_view password);
        utils::ServiceResult<void> ResetPassword(std::string_view email_or_username,
                                                 std::string_view new_password) const;
        std::optional<User> GetCurrentUser() const;
        void Logout();
        bool IsLoggedIn() const;

    private:
        std::vector<User> users_;
        std::optional<User> current_user_;
        int next_user_id_ = 4;
    };
}