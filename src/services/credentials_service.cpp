#include "services/credentials_service.hpp"

#include "app_constants.hpp"
#include "persistence/credentials_data_adapter.hpp"
#include "services/credentials_models.hpp"
#include "services/user_password_hash.hpp"

#include <format>
#include <stdexcept>

namespace services
{

CredentialsService::CredentialsService(std::shared_ptr<persistence::CredentialsDataAdapter> adapter)
    : adapter_(std::move(adapter))
{
    if (!adapter_)
    {
        throw std::invalid_argument("CredentialsDataAdapter is required");
    }
}

utils::ServiceResult<void> CredentialsService::SetPassword(int user_id,
                                                           std::string_view password) const
{
    if (user_id <= 0)
    {
        return utils::ServiceResult<void>::Failure("Invalid user id");
    }

    if (auto rules = ValidatePasswordRules(password); !rules.success)
    {
        return rules;
    }

    if (services::CredentialRecord record{user_id, internal::HashPassword(password)};
        !adapter_->Upsert(record))
    {
        return utils::ServiceResult<void>::Failure("Failed to save password");
    }

    return utils::ServiceResult<void>::Success("Password saved successfully");
}

utils::ServiceResult<void> CredentialsService::ValidatePassword(int user_id,
                                                                std::string_view password) const
{
    if (user_id <= 0)
    {
        return utils::ServiceResult<void>::Failure("Invalid user id");
    }

    auto stored = adapter_->FindByUserId(user_id);
    if (!stored)
    {
        return utils::ServiceResult<void>::Failure("Invalid password");
    }

    if (const auto hash = internal::HashPassword(password); stored->password_hash != hash)
    {
        return utils::ServiceResult<void>::Failure("Invalid password");
    }

    return utils::ServiceResult<void>::Success("Password is valid");
}

utils::ServiceResult<void> CredentialsService::ValidatePasswordRules(std::string_view password)
{
    if (password.empty())
    {
        return utils::ServiceResult<void>::Failure("Password cannot be empty");
    }

    if (password.size() < app::kPasswordMinLength)
    {
        return utils::ServiceResult<void>::Failure(
            std::format("Password must be at least {} characters", app::kPasswordMinLength));
    }

    if (password.size() > app::kPasswordMaxLength)
    {
        return utils::ServiceResult<void>::Failure(
            std::format("Password must be at most {} characters", app::kPasswordMaxLength));
    }

    return utils::ServiceResult<void>::Success("Password is valid");
}

} // namespace services
