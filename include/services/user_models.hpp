#pragma once

#include <string>

namespace services
{

struct User
{
    int id;
    std::string username;
    std::string full_name;
    std::string email;
};

struct UserRecord
{
    int id;
    std::string username;
    std::string full_name;
    std::string email;
};

} // namespace services
