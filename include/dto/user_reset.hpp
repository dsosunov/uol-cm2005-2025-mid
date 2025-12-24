#pragma once
#include <string>
namespace dto
{
struct UserReset
{
    std::string email_or_username;
    std::string new_password;
};
} // namespace dto