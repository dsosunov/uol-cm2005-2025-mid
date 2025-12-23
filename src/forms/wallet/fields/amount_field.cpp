#include "forms/wallet/fields/amount_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/wallet_operation.hpp"
#include "forms/wallet/validators/amount_validator.hpp"

namespace wallet_forms
{

    AmountField::AmountField()
        : form::TextField(
              "amount", "Amount",
              form::SimpleFieldBinder<dto::WalletOperation, std::string>(&dto::WalletOperation::amount),
              std::make_shared<AmountValidator>()) {}

} // namespace wallet_forms
