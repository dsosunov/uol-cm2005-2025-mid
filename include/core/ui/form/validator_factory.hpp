#pragma once
#include <memory>
#include "core/ui/form/validator.hpp"
namespace user_forms
{
    class EmailValidator;
    class UsernameValidator;
    class PasswordValidator;
    class FullNameValidator;
    class EmailOrUsernameValidator;
}
namespace wallet_forms
{
    class CurrencyValidator;
    class AmountValidator;
}
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
        static std::shared_ptr<Validator> Username();
        static std::shared_ptr<Validator> Password();
        static std::shared_ptr<Validator> FullName();
        static std::shared_ptr<Validator> EmailOrUsername();
        static std::shared_ptr<Validator> Amount();
    private:
        ValidatorFactory() = default;
    };
}