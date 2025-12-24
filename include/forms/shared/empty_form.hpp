#pragma once
#include "core/ui/form/form.hpp"
#include "dto/empty_request.hpp"

namespace shared_forms
{
class EmptyForm
{
  public:
    EmptyForm() = default;

    form::FormReadResult Read(const EmptyRequest&) const
    {
        return form::FormReadResult::kSuccess;
    }
};
} // namespace shared_forms
