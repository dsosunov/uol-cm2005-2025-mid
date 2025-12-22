#include <memory>

#include "io/standard_input.hpp"
#include "io/standard_output.hpp"
#include "actions/print_hello_action.hpp"
#include "actions/show_version_action.hpp"
#include "menu/menu_builder.hpp"
#include "menu/menu_engine.hpp"
#include "menu/menu_input.hpp"
#include "menu/menu_renderer.hpp"

std::unique_ptr<MenuNode> BuildMenu()
{
    return MenuBuilder("Main Menu")
        .AddBranch("Tools")
            .AddLeaf("Say Hello", std::make_unique<PrintHelloAction>())
            .AddLeaf("Show Version", std::make_unique<ShowVersionAction>())
            .Parent()
        .AddBranch("Settings")
            .AddLeaf("Reset to Defaults", std::make_unique<PrintHelloAction>())
            .Parent()
        .Build();
}

int main()
{
    auto menu = BuildMenu();
    StandardOutput output;
    StandardInput input;
    MenuRenderer renderer(output);
    MenuInput menuInput(input, renderer);
    MenuEngine engine(std::move(menu), renderer, menuInput);
    engine.Run();
    return 0;
}
