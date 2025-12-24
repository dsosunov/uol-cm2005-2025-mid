#include "forms/transaction/currency_form.hpp"

#include "core/ui/form/form_builder_utils.hpp"
#include "forms/transaction/fields/currency_field.hpp"

namespace transaction_forms
{
CurrencyForm::CurrencyForm(std::shared_ptr<form::FormInputProvider> input_provider,
                           std::shared_ptr<Output> output,
                           const std::set<std::string, std::less<>>& allowed_currencies)
    : form::Form(SetupFormLayout(allowed_currencies), input_provider, output)
{
}

std::vector<std::shared_ptr<form::Field>> CurrencyForm::SetupFormLayout(
    const std::set<std::string, std::less<>>& allowed_currencies)
{
    std::vector<std::shared_ptr<form::Field>> fields;
    form::AddField<CurrencyField>(fields, allowed_currencies);

    return fields;
}
} // namespace transaction_forms