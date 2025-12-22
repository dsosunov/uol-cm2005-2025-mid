#include <memory>

#include "core/ui/io/standard_input.hpp"
#include "core/ui/io/standard_output.hpp"
#include "core/ui/form/menu_form_input_provider.hpp"
#include "core/actions/action_context.hpp"
#include "actions/print_hello_action.hpp"
#include "actions/show_version_action.hpp"
#include "actions/product_query_action.hpp"
#include "core/ui/menu/menu_builder.hpp"
#include "core/ui/menu/menu_engine.hpp"
#include "core/ui/menu/menu_input.hpp"
#include "core/ui/menu/menu_renderer.hpp"

std::unique_ptr<MenuNode> BuildMenu()
{
    return MenuBuilder("Main Menu")
        .AddBranch("Tools")
            .AddLeaf("Say Hello", std::make_unique<PrintHelloAction>())
            .AddLeaf("Show Version", std::make_unique<ShowVersionAction>())
            .AddLeaf("Product Query", std::make_unique<ProductQueryAction>())
            .Parent()
        .AddBranch("Settings")
            .AddLeaf("Reset to Defaults", std::make_unique<PrintHelloAction>())
            .Parent()
        .Build();
}

int main()
{
    auto output = std::make_shared<StandardOutput>();
    auto input = std::make_shared<StandardInput>();
    auto renderer = std::make_shared<MenuRenderer>(output);
    auto menuInput = std::make_shared<MenuInput>(input, renderer);

    // Create FormInputProvider
    auto formInputProvider = std::make_shared<form::MenuFormInputProvider>(
        input, output, renderer, menuInput);

    // Create ActionContext with dependencies
    auto context = std::make_shared<ActionContext>(output, formInputProvider);

    auto menu = BuildMenu();
    MenuEngine engine(std::move(menu), renderer, menuInput, context);
    engine.Run();
    return 0;
}
