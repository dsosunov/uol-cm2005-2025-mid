#include "core/ui/form/validator_factory.hpp"

#include "forms/user/validators/user_validators.hpp"
#include "forms/wallet/validators/amount_validator.hpp"

namespace form
{
std::shared_ptr<Validator> ValidatorFactory::Email()
{
    static auto instance = std::make_shared<user_forms::EmailValidator>();
    return instance;
}

std::shared_ptr<Validator> ValidatorFactory::Password()
{
    static auto instance = std::make_shared<user_forms::PasswordValidator>();
    return instance;
}

std::shared_ptr<Validator> ValidatorFactory::FullName()
{
    static auto instance = std::make_shared<user_forms::FullNameValidator>();
    return instance;
}

std::shared_ptr<Validator> ValidatorFactory::Amount()
{
    static auto instance = std::make_shared<wallet_forms::AmountValidator>();
    return instance;
}
} // namespace form