#pragma once
#include "core/actions/form_based_action.hpp"
#include "services/user_service.hpp"
#include "dto/user_registration.hpp"
#include "forms/user/registration_form.hpp"
#include <memory>
class UserRegisterAction : public actions::FormBasedAction<user_forms::RegistrationForm, dto::UserRegistration, utils::ServiceResult<services::User>>
{
public:
  explicit UserRegisterAction(std::shared_ptr<services::UserService> user_service);

protected:
  user_forms::RegistrationForm CreateForm(ActionContext &context) override;
  utils::ServiceResult<services::User> ExecuteService(const dto::UserRegistration &data, ActionContext &context) override;
  void DisplayResults(const utils::ServiceResult<services::User> &result, const dto::UserRegistration &data, ActionContext &context) override;
  const char *GetOperationName() const override { return "Registration"; }

private:
  std::shared_ptr<services::UserService> user_service_;
};