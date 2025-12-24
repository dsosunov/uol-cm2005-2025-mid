#include "forms/wallet/wallet_operation_form.hpp"

#include "core/ui/form/form_builder_utils.hpp"
#include "forms/wallet/fields/amount_field.hpp"
#include "forms/wallet/fields/currency_field.hpp"

namespace wallet_forms
{
WalletOperationForm::WalletOperationForm(
    std::shared_ptr<form::FormInputProvider> input_provider, std::shared_ptr<Output> output,
    const std::set<std::string, std::less<>>& allowed_currencies)
    : form::Form(SetupFormLayout(allowed_currencies), input_provider, output)
{
}

std::vector<std::shared_ptr<form::Field>> WalletOperationForm::SetupFormLayout(
    const std::set<std::string, std::less<>>& allowed_currencies)
{
    std::vector<std::shared_ptr<form::Field>> fields;
    form::AddField<CurrencyField>(fields, allowed_currencies);
    form::AddField<AmountField>(fields);

    return fields;
}
} // namespace wallet_forms