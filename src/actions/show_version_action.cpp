#include "actions/show_version_action.hpp"

void ShowVersionAction::Execute(ActionContext& context)
{
    context.output->WriteLine("Version: 1.0.0");
}
