#pragma once
#include "core/ui/form/validator.hpp"

#include <memory>

namespace user_forms
{
class EmailValidator;
class PasswordValidator;
class FullNameValidator;
} // namespace user_forms
namespace wallet_forms
{
class CurrencyValidator;
class AmountValidator;
} // namespace wallet_forms
namespace forms::shared
{
class CurrencyPairValidator;
}
namespace form
{
class ValidatorFactory
{
  public:
    static std::shared_ptr<Validator> Email();
    static std::shared_ptr<Validator> Password();
    static std::shared_ptr<Validator> FullName();
    static std::shared_ptr<Validator> Amount();

  private:
    ValidatorFactory() = default;
};
} // namespace form