#pragma once
#include <string>

#include "core/ui/form/form_context.hpp"
#include "core/ui/form/validation_result.hpp"

namespace user_forms {

class EmailValidator {
 public:
  form::ValidationResult operator()(const std::string& value,
                                    const form::FormContext& context) const;
};

class UsernameValidator {
 public:
  form::ValidationResult operator()(const std::string& value,
                                    const form::FormContext& context) const;
};

class PasswordValidator {
 public:
  form::ValidationResult operator()(const std::string& value,
                                    const form::FormContext& context) const;
};

class FullNameValidator {
 public:
  form::ValidationResult operator()(const std::string& value,
                                    const form::FormContext& context) const;
};

class EmailOrUsernameValidator {
 public:
  form::ValidationResult operator()(const std::string& value,
                                    const form::FormContext& context) const;
};

}  // namespace user_forms
