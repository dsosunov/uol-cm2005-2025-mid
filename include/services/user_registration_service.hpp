#pragma once

#include "core/utils/service_result.hpp"
#include "services/user_models.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace persistence
{
class UserDataAdapter;
}

namespace services
{

class WalletService;

class UserRegistrationService
{
  public:
    UserRegistrationService(std::shared_ptr<persistence::UserDataAdapter> adapter,
                            std::shared_ptr<services::WalletService> wallet_service);
    ~UserRegistrationService() = default;

    std::string GenerateUsername() const;

    utils::ServiceResult<User> RegisterUser(std::string_view username, std::string_view full_name,
                                            std::string_view email,
                                            std::string_view password) const;

  private:
    std::shared_ptr<persistence::UserDataAdapter> adapter_;
    std::shared_ptr<services::WalletService> wallet_service_;

    static User ToUser(const UserRecord& record);
};

} // namespace services
