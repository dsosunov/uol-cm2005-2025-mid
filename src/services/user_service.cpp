#include "services/user_service.hpp"
namespace services
{
    UserService::UserService()
    {
        users_.emplace_back(1, "John Doe", "john@example.com", "john_doe");
        users_.emplace_back(2, "Jane Smith", "jane@example.com", "jane_smith");
        users_.emplace_back(3, "Bob Wilson", "bob@example.com", "bob_wilson");
    }
    utils::ServiceResult<User> UserService::RegisterUser(std::string_view full_name,
                                                         std::string_view email,
                                                         [[maybe_unused]] std::string_view password)
    {
        for (const auto &user : users_)
        {
            if (user.email == email)
            {
                return utils::ServiceResult<User>::Failure("Email already registered");
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
        return utils::ServiceResult<User>::Success(new_user, "Registration successful");
    }
    utils::ServiceResult<User> UserService::LoginUser(std::string_view username, std::string_view password)
    {
        for (const auto &user : users_)
        {
            if (user.username == username || user.email == username)
            {
                if (!password.empty())
                {
                    current_user_ = user;
                    return utils::ServiceResult<User>::Success(user, "Login successful");
                }
                return utils::ServiceResult<User>::Failure("Invalid password");
            }
        }
        return utils::ServiceResult<User>::Failure("User not found");
    }
    utils::ServiceResult<void> UserService::ResetPassword(std::string_view email_or_username,
                                                          [[maybe_unused]] std::string_view new_password) const
    {
        for (const auto &user : users_)
        {
            if (user.username == email_or_username || user.email == email_or_username)
            {
                return utils::ServiceResult<void>::Success("Password reset successful");
            }
        }
        return utils::ServiceResult<void>::Failure("User not found");
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