#include "forms/wallet/fields/amount_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"
#include "dto/wallet_operation.hpp"

namespace wallet_forms
{

AmountField::AmountField()
    : form::TextField("amount", "Enter amount",
                      form::SimpleFieldBinder<dto::WalletOperation, std::string>(
                          &dto::WalletOperation::amount)) {}

}  // namespace wallet_forms
