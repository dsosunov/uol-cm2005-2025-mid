#pragma once
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

    struct RegistrationResult
    {
        bool success;
        std::string message;
        std::optional<User> user;
    };

    struct LoginResult
    {
        bool success;
        std::string message;
        std::optional<User> user;
    };

    struct ResetResult
    {
        bool success;
        std::string message;
    };

    class UserService
    {
    public:
        UserService();
        ~UserService() = default;

        // User operations
        RegistrationResult RegisterUser(std::string_view full_name, std::string_view email,
                                        std::string_view password);
        LoginResult LoginUser(std::string_view username, std::string_view password);
        ResetResult ResetPassword(std::string_view email_or_username,
                                  std::string_view new_password) const;

        // Session management
        std::optional<User> GetCurrentUser() const;
        void Logout();
        bool IsLoggedIn() const;

    private:
        std::vector<User> users_;
        std::optional<User> current_user_;
        int next_user_id_ = 4;
    };

} // namespace services
