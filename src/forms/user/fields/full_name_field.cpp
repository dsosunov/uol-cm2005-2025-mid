#include "forms/user/fields/full_name_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "core/ui/form/validator_factory.hpp"
#include "dto/user_registration.hpp"

namespace user_forms
{

FullNameField::FullNameField()
    : form::TextField("full_name", "Full name",
                      form::SimpleFieldBinder<dto::UserRegistration, std::string>(
                          &dto::UserRegistration::full_name),
                      form::ValidatorFactory::FullName())
{
}

} // namespace user_forms