#include "menu_renderer.hpp"
#include <string>

MenuRenderer::MenuRenderer(IOutput& output) : _output(output) {}

void MenuRenderer::RenderMenu(const MenuNode& current) const
{
    if (!current.IsRoot())
    {
        RenderBreadcrumbs(current);
    }

    _output.WriteLine("=== " + current.Title() + " ===");

    const auto& items = current.Children();
    for (size_t i = 0; i < items.size(); ++i)
    {
        _output.WriteLine(std::to_string(i + 1) + ") " + items[i]->Title());
    }

    _output.WriteLine("0) " + std::string(current.IsRoot() ? "Exit" : "Back"));
    _output.Write("Select option (0-" + std::to_string(items.size()) + "): ");
}

void MenuRenderer::RenderInvalidChoice() const
{
    _output.WriteLine("Invalid choice.");
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
    _output.WriteLine(breadcrumb);
}
