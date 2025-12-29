#pragma once

#include "core/utils/service_result.hpp"
#include "services/authentication_context.hpp"

#include <memory>
#include <stdexcept>

namespace services
{

class AuthenticationService
{
  public:
    explicit AuthenticationService(std::shared_ptr<AuthenticationContext> context)
        : context_(std::move(context))
    {
        if (!context_)
        {
            throw std::invalid_argument("AuthenticationContext is required");
        }
    }

    bool IsAuthenticated() const
    {
        return context_->IsAuthenticated();
    }

    utils::ServiceResult<AuthenticatedUser> GetCurrentUser() const
    {
        return utils::ServiceResult<AuthenticatedUser>::Success(
            context_->GetCurrentUser(), context_->IsAuthenticated() ? "Authenticated" : "Guest");
    }

    utils::ServiceResult<AuthenticatedUser> GetAuthenticatedUser() const
    {
        if (!context_->IsAuthenticated())
        {
            return utils::ServiceResult<AuthenticatedUser>::Failure("Please log in first");
        }

        return utils::ServiceResult<AuthenticatedUser>::Success(context_->GetCurrentUser(),
                                                                "Authenticated");
    }

    void SetCurrentUser(const AuthenticatedUser& user)
    {
        context_->SetCurrentUser(user);
    }

    void Logout()
    {
        context_->SetGuestUser();
    }

  private:
    std::shared_ptr<AuthenticationContext> context_;
};

} // namespace services
