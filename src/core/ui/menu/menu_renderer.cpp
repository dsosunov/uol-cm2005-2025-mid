#include "core/ui/menu/menu_renderer.hpp"

#include "core/utils/output_formatter.hpp"

#include <format>
#include <string>

MenuRenderer::MenuRenderer(std::shared_ptr<Output> output) : output_(std::move(output))
{
}

void MenuRenderer::RenderMenu(const MenuNode& current, std::optional<size_t> default_option) const
{
    output_->WriteLine("");

    std::string title;
    if (!current.IsRoot())
    {
        title = BuildBreadcrumbPath(current);
    }
    else
    {
        title = current.Title();
    }

    output_->WriteLine(utils::OutputFormatter::SectionHeader(title));

    const auto& items = current.Children();
    for (size_t i = 0; i < items.size(); ++i)
    {
        const size_t index = i + 1;
        const bool is_default = default_option.has_value() && *default_option == index;
        output_->WriteLine(
            std::format("{}) {}{}", index, items[i]->Title(), is_default ? " [default]" : ""));
    }

    output_->WriteLine(std::format("0) {}", current.IsRoot() ? "Exit" : "Back"));
    output_->WriteLine("───────────────────────────────────────");

    if (default_option.has_value() && *default_option >= 1 && *default_option <= items.size())
    {
        const auto& label = items[*default_option - 1]->Title();
        output_->Write(std::format("Choose an option (0-{}). Press Enter for default ({}: {}): ",
                                   items.size(), *default_option, label));
    }
    else
    {
        output_->Write(std::format("Choose an option (0-{}): ", items.size()));
    }
}

void MenuRenderer::RenderActionHeader(const MenuNode& node) const
{
}

void MenuRenderer::RenderInvalidChoice() const
{
    output_->WriteLine("Invalid choice.");
}

std::string MenuRenderer::BuildBreadcrumbPath(const MenuNode& current) const
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

    return breadcrumb;
}