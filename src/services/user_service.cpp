#include "services/user_service.hpp"

namespace services
{

    UserService::UserService() : next_user_id_(1)
    {
        // Pre-populate with some test users
        users_.push_back({1, "John Doe", "john@example.com", "john_doe"});
        users_.push_back({2, "Jane Smith", "jane@example.com", "jane_smith"});
        users_.push_back({3, "Bob Wilson", "bob@example.com", "bob_wilson"});
        next_user_id_ = 4;
    }

    RegistrationResult UserService::RegisterUser(const std::string &full_name,
                                                 const std::string &email,
                                                 const std::string &password)
    {
        // Check if email already exists
        for (const auto &user : users_)
        {
            if (user.email == email)
            {
                return {false, "Email already registered", std::nullopt};
            }
        }

        // Create username from email (part before @)
        std::string username = email.substr(0, email.find('@'));

        // Check if username already exists
        for (const auto &user : users_)
        {
            if (user.username == username)
            {
                username += std::to_string(next_user_id_);
                break;
            }
        }

        // Create new user
        User new_user{next_user_id_++, full_name, email, username};
        users_.push_back(new_user);

        return {true, "Registration successful", new_user};
    }

    LoginResult UserService::LoginUser(const std::string &username, const std::string &password)
    {
        // Find user by username or email
        for (const auto &user : users_)
        {
            if (user.username == username || user.email == username)
            {
                // In a real system, we'd check password hash here
                // For now, just accept any non-empty password
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

    ResetResult UserService::ResetPassword(const std::string &email_or_username,
                                           const std::string &new_password)
    {
        // Find user by username or email
        for (const auto &user : users_)
        {
            if (user.username == email_or_username || user.email == email_or_username)
            {
                // In a real system, we'd update the password hash
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

} // namespace services
