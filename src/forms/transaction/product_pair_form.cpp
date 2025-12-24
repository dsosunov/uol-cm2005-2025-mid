#include "forms/transaction/product_pair_form.hpp"

#include "core/ui/form/form_builder_utils.hpp"
#include "forms/transaction/fields/product_pair_field.hpp"

namespace transaction_forms
{
ProductPairForm::ProductPairForm(std::shared_ptr<form::FormInputProvider> input_provider,
                                 std::shared_ptr<Output> output,
                                 const std::set<std::string, std::less<>>& allowed_currencies)
    : form::Form(SetupFormLayout(allowed_currencies), input_provider, output)
{
}
std::vector<std::shared_ptr<form::Field>> ProductPairForm::SetupFormLayout(
    const std::set<std::string, std::less<>>& allowed_currencies)
{
    std::vector<std::shared_ptr<form::Field>> fields;
    form::AddField<ProductPairField>(fields, allowed_currencies);
    return fields;
}
} // namespace transaction_forms