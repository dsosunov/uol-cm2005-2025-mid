#include "forms/wallet/fields/currency_field.hpp"
#include "forms/wallet/validators/currency_validator.hpp"
#include "core/ui/form/binders/field_binders.hpp"
#include "dto/wallet_operation.hpp"

namespace wallet_forms
{

CurrencyField::CurrencyField(std::set<std::string> allowed_currencies)
    : form::TextField("currency", "Enter currency code (e.g., USD)",
                      form::SimpleFieldBinder<dto::WalletOperation, std::string>(
                          &dto::WalletOperation::currency),
                      CurrencyValidator(std::move(allowed_currencies))) {}

}  // namespace wallet_forms
