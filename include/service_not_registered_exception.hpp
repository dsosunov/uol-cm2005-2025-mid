#pragma once
#include <stdexcept>
#include <string>
class ServiceNotRegisteredException : public std::runtime_error
{
public:
    explicit ServiceNotRegisteredException(const std::string &service_name)
        : std::runtime_error("Service not registered: " + service_name)
    {
    }
};
