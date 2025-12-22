#pragma once
#include <string>

namespace dto {

struct UserRegistration {
  std::string full_name;
  std::string email;
  std::string password;
};

}  // namespace dto
