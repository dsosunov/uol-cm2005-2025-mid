#include "services/user_service.hpp"

namespace services
{

    UserService::UserService()
    {
        users_.emplace_back(1, "John Doe", "john@example.com", "john_doe");
        users_.emplace_back(2, "Jane Smith", "jane@example.com", "jane_smith");
        users_.emplace_back(3, "Bob Wilson", "bob@example.com", "bob_wilson");
    }

    RegistrationResult UserService::RegisterUser(std::string_view full_name,
                                                 std::string_view email,
                                                 [[maybe_unused]] std::string_view password)
    {
        for (const auto &user : users_)
        {
            if (user.email == email)
            {
                return {false, "Email already registered", std::nullopt};
            }
        }
        std::string username(email.substr(0, email.find('@')));
        for (const auto &user : users_)
        {
            if (user.username == username)
            {
                username += std::to_string(next_user_id_);
                break;
            }
        }
        User new_user{next_user_id_++, std::string(full_name), std::string(email), username};
        users_.emplace_back(new_user);

        return {true, "Registration successful", new_user};
    }

    LoginResult UserService::LoginUser(std::string_view username, std::string_view password)
    {
        for (const auto &user : users_)
        {
            if (user.username == username || user.email == username)
            {
                if (!password.empty())
                {
                    current_user_ = user;
                    return {true, "Login successful", user};
                }
                return {false, "Invalid password", std::nullopt};
            }
        }

        return {false, "User not found", std::nullopt};
    }

    ResetResult UserService::ResetPassword(std::string_view email_or_username,
                                           [[maybe_unused]] std::string_view new_password) const
    {
        for (const auto &user : users_)
        {
            if (user.username == email_or_username || user.email == email_or_username)
            {
                return {true, "Password reset successful"};
            }
        }

        return {false, "User not found"};
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

}
