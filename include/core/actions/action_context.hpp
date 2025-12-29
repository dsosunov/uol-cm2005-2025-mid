#pragma once
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"
#include "services/authentication_service.hpp"

#include <memory>
#include <stdexcept>

struct ActionContext
{
    std::shared_ptr<Output> output;
    std::shared_ptr<form::FormInputProvider> form_input_provider;
    std::shared_ptr<services::AuthenticationService> auth_service;

    ActionContext(std::shared_ptr<Output> out, std::shared_ptr<form::FormInputProvider> provider,
                  std::shared_ptr<services::AuthenticationService> authentication)
        : output(std::move(out)), form_input_provider(std::move(provider)),
          auth_service(std::move(authentication))
    {
        if (!auth_service)
        {
            throw std::invalid_argument("AuthenticationService is required");
        }
    }
};