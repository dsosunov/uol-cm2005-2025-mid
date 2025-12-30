#pragma once

#include "core/utils/service_result.hpp"

#include <memory>
#include <string_view>

namespace persistence
{
class CredentialsDataAdapter;
}

namespace services
{

class CredentialsService
{
  public:
    explicit CredentialsService(std::shared_ptr<persistence::CredentialsDataAdapter> adapter);

    utils::ServiceResult<void> SetPassword(int user_id, std::string_view password) const;
    utils::ServiceResult<void> ValidatePassword(int user_id, std::string_view password) const;

  private:
    std::shared_ptr<persistence::CredentialsDataAdapter> adapter_;

    static utils::ServiceResult<void> ValidatePasswordRules(std::string_view password);
};

} // namespace services
