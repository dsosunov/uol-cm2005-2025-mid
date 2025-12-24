#include "core/actions/action_helper.hpp"
namespace actions
{
bool ActionHelper::HandleFormCancellation(form::FormReadResult result,
                                          std::string_view operation_name, ActionContext& context)
{
    if (result == form::FormReadResult::kCancelled)
    {
        context.output->WriteLine("");
        context.output->WriteLine(utils::OutputFormatter::CancellationMessage(operation_name));
        return true;
    }
    return false;
}
void ActionHelper::DisplayResult(bool success, std::string_view operation_name,
                                 std::string_view message, ActionContext& context,
                                 const std::function<void()>& display_data)
{
    context.output->WriteLine("");
    if (success)
    {
        context.output->WriteLine(utils::OutputFormatter::SuccessHeader(operation_name));
        if (display_data)
        {
            display_data();
        }
    }
    else
    {
        context.output->WriteLine(utils::OutputFormatter::FailureHeader(operation_name));
        if (!message.empty())
        {
            context.output->WriteLine(std::format("Error: {}", message));
        }
    }
}
void ActionHelper::DisplaySectionHeader(std::string_view title, ActionContext& context)
{
    context.output->WriteLine(utils::OutputFormatter::SectionHeader(title));
}
} // namespace actions