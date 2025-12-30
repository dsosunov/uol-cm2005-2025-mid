#include "core/ui/menu/menu_renderer.hpp"

#include "core/utils/output_formatter.hpp"
#include "services/authentication_service.hpp"

#include <format>
#include <string>

MenuRenderer::MenuRenderer(std::shared_ptr<Output> output,
                           std::shared_ptr<services::AuthenticationService> auth_service)
    : output_(std::move(output)), auth_service_(std::move(auth_service))
{
}

void MenuRenderer::RenderMenu(const MenuNode& current, std::optional<size_t> default_option) const
{
    RenderMenu(current, std::string_view{}, default_option);
}

void MenuRenderer::RenderMenu(const MenuNode& current, std::string_view status_line,
                              std::optional<size_t> default_option) const
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

    if (!status_line.empty())
    {
        output_->WriteLine(status_line);
    }

    const bool is_authenticated = auth_service_ && auth_service_->IsAuthenticated();

    std::vector<const MenuNode*> visible_items;
    visible_items.reserve(current.Children().size());
    for (const auto& child : current.Children())
    {
        if (child->IsVisibleTo(is_authenticated))
        {
            visible_items.push_back(child.get());
        }
    }

    for (size_t i = 0; i < visible_items.size(); ++i)
    {
        const size_t index = i + 1;
        const bool is_default = default_option.has_value() && *default_option == index;
        output_->WriteLine(std::format("{}) {}{}", index, visible_items[i]->Title(),
                                       is_default ? " [default]" : ""));
    }

    output_->WriteLine(std::format("0) {}", current.IsRoot() ? "Exit" : "Back"));
    output_->WriteLine("───────────────────────────────────────");

    if (default_option.has_value() && *default_option >= 1 &&
        *default_option <= visible_items.size())
    {
        const auto& label = visible_items[*default_option - 1]->Title();
        output_->Write(std::format("Choose an option (0-{}). Press Enter for default ({}: {}): ",
                                   visible_items.size(), *default_option, label));
    }
    else
    {
        output_->Write(std::format("Choose an option (0-{}): ", visible_items.size()));
    }
}

void MenuRenderer::RenderActionHeader([[maybe_unused]] const MenuNode& node) const
{
    // Intentionally empty: action header rendering is not used in the current UI.
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