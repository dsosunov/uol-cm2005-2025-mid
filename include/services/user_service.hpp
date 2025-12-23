#pragma once
#include <memory>
#include <optional>
#include <string>
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
        RegistrationResult RegisterUser(const std::string &full_name, const std::string &email,
                                        const std::string &password);
        LoginResult LoginUser(const std::string &username, const std::string &password);
        ResetResult ResetPassword(const std::string &email_or_username,
                                  const std::string &new_password);

        // Session management
        std::optional<User> GetCurrentUser() const;
        void Logout();
        bool IsLoggedIn() const;

    private:
        std::vector<User> users_;
        std::optional<User> current_user_;
        int next_user_id_;
    };

} // namespace services
