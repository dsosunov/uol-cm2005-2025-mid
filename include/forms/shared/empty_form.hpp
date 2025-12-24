#pragma once
#include "core/ui/form/form.hpp"
#include "dto/empty_request.hpp"

namespace shared_forms
{
// Empty form for actions that don't require input
class EmptyForm
{
  public:
    EmptyForm() = default;

    form::FormReadResult Read(const EmptyRequest&) const
    {
        // No input needed
        return form::FormReadResult::kSuccess;
    }
};
} // namespace shared_forms
