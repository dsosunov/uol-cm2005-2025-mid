#include "forms/wallet/fields/currency_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"
#include "dto/wallet_operation.hpp"
#include "forms/wallet/validators/currency_validator.hpp"
namespace wallet_forms
{
    CurrencyField::CurrencyField(std::set<std::string, std::less<>> allowed_currencies)
        : form::TextField("currency", "Currency",
                          form::SimpleFieldBinder<dto::WalletOperation, std::string>(
                              &dto::WalletOperation::currency),
                          std::make_shared<CurrencyValidator>(std::move(allowed_currencies))) {}
}