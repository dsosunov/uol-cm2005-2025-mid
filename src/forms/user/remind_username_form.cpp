#include "forms/user/remind_username_form.hpp"

#include "core/ui/form/form_builder_utils.hpp"
#include "forms/user/fields/remind_email_field.hpp"

namespace user_forms
{
RemindUsernameForm::RemindUsernameForm(std::shared_ptr<form::FormInputProvider> input_provider,
                                       std::shared_ptr<Output> output)
    : form::Form(SetupFormLayout(), input_provider, output)
{
}

std::vector<std::shared_ptr<form::Field>> RemindUsernameForm::SetupFormLayout()
{
    std::vector<std::shared_ptr<form::Field>> fields;
    form::AddField<RemindEmailField>(fields);

    return fields;
}
} // namespace user_forms
