#include "forms/user/fields/full_name_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/user_registration.hpp"
#include "forms/user/validators/user_validators.hpp"

namespace user_forms
{

    FullNameField::FullNameField()
        : form::TextField("full_name", "Full name",
                          form::SimpleFieldBinder<dto::UserRegistration, std::string>(
                              &dto::UserRegistration::full_name),
                          std::make_shared<FullNameValidator>()) {}

}
