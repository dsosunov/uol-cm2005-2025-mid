#pragma once

#include <string>

namespace services
{

struct AuthenticatedUser
{
    int id;
    std::string username;
    std::string full_name;
    std::string email;
};

class AuthenticationContext
{
  public:
    AuthenticationContext()
    {
        SetGuestUser();
    }

    bool IsAuthenticated() const
    {
        return is_authenticated_;
    }

    const AuthenticatedUser& GetCurrentUser() const
    {
        return current_user_;
    }

    void SetCurrentUser(AuthenticatedUser user)
    {
        current_user_ = std::move(user);
        is_authenticated_ = true;
    }

    void SetGuestUser(int guest_user_id = 0)
    {
        current_user_ = AuthenticatedUser{guest_user_id, "", "", ""};
        is_authenticated_ = false;
    }

  private:
    AuthenticatedUser current_user_{};
    bool is_authenticated_ = false;
};

} // namespace services
