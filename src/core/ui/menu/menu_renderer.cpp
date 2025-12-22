#include "core/ui/menu/menu_renderer.hpp"
#include <string>

MenuRenderer::MenuRenderer(Output& output) : output_(output) {}

void MenuRenderer::RenderMenu(const MenuNode& current) const
{
    if (!current.IsRoot())
    {
        RenderBreadcrumbs(current);
    }

    output_.WriteLine("=== " + current.Title() + " ===");

    const auto& items = current.Children();
    for (size_t i = 0; i < items.size(); ++i)
    {
        output_.WriteLine(std::to_string(i + 1) + ") " + items[i]->Title());
    }

    output_.WriteLine("0) " + std::string(current.IsRoot() ? "Exit" : "Back"));
    output_.Write("Select option (0-" + std::to_string(items.size()) + "): ");
}

void MenuRenderer::RenderInvalidChoice() const
{
    output_.WriteLine("Invalid choice.");
}

void MenuRenderer::RenderBreadcrumbs(const MenuNode& current) const
{
    std::vector<const MenuNode*> path;
    const MenuNode* node = &current;

    while (node)
    {
        path.push_back(node);
        node = node->Parent();
    }

    std::string breadcrumb;
    for (auto it = path.rbegin(); it != path.rend(); ++it)
    {
        if (it != path.rbegin())
        {
            breadcrumb += " > ";
        }
        breadcrumb += (*it)->Title();
    }
    output_.WriteLine(breadcrumb);
}
