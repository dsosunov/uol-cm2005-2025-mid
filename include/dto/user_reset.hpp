#pragma once
#include <string>
namespace dto
{
struct UserReset
{
    std::string username;
    std::string new_password;
};
} // namespace dto