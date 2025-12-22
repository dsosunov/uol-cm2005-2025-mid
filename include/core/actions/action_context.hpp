#pragma once
#include <memory>

#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"

struct ActionContext {
  std::shared_ptr<Output> output;
  std::shared_ptr<form::FormInputProvider> form_input_provider;

  ActionContext(std::shared_ptr<Output> out, std::shared_ptr<form::FormInputProvider> provider)
      : output(std::move(out)), form_input_provider(std::move(provider)) {}
};
