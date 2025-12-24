#pragma once
#include <string>
namespace dto
{
struct UserReset
{
    std::string email;
    std::string new_password;
};
} // namespace dto