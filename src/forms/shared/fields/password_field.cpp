#include "forms/shared/fields/password_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "core/ui/form/validator_factory.hpp"
#include "dto/user_login.hpp"
#include "dto/user_registration.hpp"
#include "dto/user_reset.hpp"

namespace forms::shared
{
template <typename TDTO>
PasswordField<TDTO>::PasswordField(const std::string& field_name, const std::string& prompt,
                                   MemberPtr member_ptr)
    : form::TextField(field_name, prompt, form::SimpleFieldBinder<TDTO, std::string>(member_ptr),
                      form::ValidatorFactory::Password())
{
}
template class PasswordField<dto::UserRegistration>;
template class PasswordField<dto::UserLogin>;
template class PasswordField<dto::UserReset>;
} // namespace forms::shared