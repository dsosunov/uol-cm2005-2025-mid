#include "services/user_registration_service.hpp"

#include "app_constants.hpp"
#include "persistence/user_data_adapter.hpp"
#include "services/credentials_service.hpp"
#include "services/wallet_service.hpp"

#include <cstdint>
#include <format>
#include <random>
#include <stdexcept>

namespace services
{

UserRegistrationService::UserRegistrationService(
    std::shared_ptr<persistence::UserDataAdapter> adapter,
    std::shared_ptr<services::WalletService> wallet_service,
    std::shared_ptr<services::CredentialsService> credentials_service)
    : adapter_(std::move(adapter)), wallet_service_(std::move(wallet_service)),
      credentials_service_(std::move(credentials_service))
{
    if (!adapter_)
    {
        throw std::invalid_argument("UserDataAdapter is required");
    }
    if (!wallet_service_)
    {
        throw std::invalid_argument("WalletService is required");
    }
    if (!credentials_service_)
    {
        throw std::invalid_argument("CredentialsService is required");
    }
}

std::string UserRegistrationService::GenerateUsername() const
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(0ULL, 9999999999ULL);

    for (int attempt = 0; attempt < 200; ++attempt)
    {
        std::string username = std::format("{:010}", dist(gen));
        if (!adapter_->ExistsByUsername(username))
        {
            return username;
        }
    }

    // Fallback (extremely unlikely): scan sequentially.
    for (uint64_t n = 0; n <= 9999999999ULL; ++n)
    {
        std::string username = std::format("{:010}", n);
        if (!adapter_->ExistsByUsername(username))
        {
            return username;
        }
    }

    return "0000000000";
}

utils::ServiceResult<User> UserRegistrationService::RegisterUser(std::string_view username,
                                                                 std::string_view full_name,
                                                                 std::string_view email,
                                                                 std::string_view password) const
{
    if (adapter_->ExistsByUsername(username))
    {
        return utils::ServiceResult<User>::Failure("Username already registered");
    }

    if (adapter_->ExistsByEmail(email))
    {
        return utils::ServiceResult<User>::Failure("Email already registered");
    }

    if (adapter_->ExistsByFullName(full_name))
    {
        return utils::ServiceResult<User>::Failure("Full name already registered");
    }

    UserRecord new_record{0, std::string(username), std::string(full_name), std::string(email)};

    if (!adapter_->Insert(new_record))
    {
        return utils::ServiceResult<User>::Failure("Failed to save user");
    }

    if (auto pw = credentials_service_->SetPassword(new_record.id, password); !pw.success)
    {
        return utils::ServiceResult<User>::Failure(pw.message);
    }

    for (const auto& [currency, amount] : app::kInitialBalances)
    {
        if (amount <= 0.0)
        {
            continue;
        }

        auto deposit = wallet_service_->Deposit(new_record.id, currency, amount);
        if (!deposit.success)
        {
            return utils::ServiceResult<User>::Failure(std::format(
                "Failed to initialize wallet balance for {}: {}", currency, deposit.message));
        }
    }

    return utils::ServiceResult<User>::Success(ToUser(new_record), "Registration successful");
}

User UserRegistrationService::ToUser(const UserRecord& record)
{
    return User{record.id, record.username, record.full_name, record.email};
}

} // namespace services
