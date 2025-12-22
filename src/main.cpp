#include <memory>

#include "core/ui/io/standard_input.hpp"
#include "core/ui/io/standard_output.hpp"
#include "actions/print_hello_action.hpp"
#include "actions/show_version_action.hpp"
#include "actions/product_query_action.hpp"
#include "core/ui/menu/menu_builder.hpp"
#include "core/ui/menu/menu_engine.hpp"
#include "core/ui/menu/menu_input.hpp"
#include "core/ui/menu/menu_renderer.hpp"

std::unique_ptr<MenuNode> BuildMenu(std::shared_ptr<Input> input,
                                     std::shared_ptr<Output> output,
                                     MenuRenderer& renderer,
                                     MenuInput& menu_input)
{
    return MenuBuilder("Main Menu")
        .AddBranch("Tools")
            .AddLeaf("Say Hello", std::make_unique<PrintHelloAction>())
            .AddLeaf("Show Version", std::make_unique<ShowVersionAction>())
            .AddLeaf("Product Query", std::make_unique<ProductQueryAction>(input, output, renderer, menu_input))
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
    MenuRenderer renderer(*output);
    MenuInput menuInput(*input, renderer);
    auto menu = BuildMenu(input, output, renderer, menuInput);
    MenuEngine engine(std::move(menu), renderer, menuInput);
    engine.Run();
    return 0;
}
