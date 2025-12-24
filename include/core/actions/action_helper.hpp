#pragma once
#include "core/actions/action_context.hpp"
#include "core/ui/form/form.hpp"
#include "core/utils/output_formatter.hpp"

#include <format>
#include <functional>
#include <string>
#include <string_view>

namespace actions
{
class ActionHelper
{
  public:
    static bool HandleFormCancellation(form::FormReadResult result, std::string_view operation_name,
                                       ActionContext& context);
    static void DisplayResult(bool success, std::string_view operation_name,
                              std::string_view message, ActionContext& context,
                              const std::function<void()>& display_data = nullptr);
    static void DisplaySectionHeader(std::string_view title, ActionContext& context);
    template <typename T>
    static void DisplayField(std::string_view label, const T& value, ActionContext& context)
    {
        context.output->WriteLine(std::format("{}: {}", label, value));
    }
};
} // namespace actions