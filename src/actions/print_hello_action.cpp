#include "actions/print_hello_action.hpp"

void PrintHelloAction::Execute(ActionContext& context)
{
    context.output->WriteLine("Hello!");
}
