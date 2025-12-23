#include "forms/wallet/fields/amount_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"
#include "core/ui/form/validator_factory.hpp"
#include "dto/wallet_operation.hpp"
namespace wallet_forms
{
    AmountField::AmountField()
        : form::TextField(
              "amount", "Amount",
              form::SimpleFieldBinder<dto::WalletOperation, std::string>(&dto::WalletOperation::amount),
              form::ValidatorFactory::Amount()) {}
}