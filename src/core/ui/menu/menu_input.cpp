#include "core/ui/menu/menu_input.hpp"

#include "services/authentication_service.hpp"

#include <algorithm>
#include <cctype>
#include <ranges>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

MenuInput::MenuInput(std::shared_ptr<Input> input, std::shared_ptr<MenuRenderer> renderer,
                     std::shared_ptr<services::AuthenticationService> auth_service)
    : input_(std::move(input)), renderer_(std::move(renderer)),
      auth_service_(std::move(auth_service))
{
}

bool MenuInput::IsBlankLine(std::string_view s)
{
    return std::ranges::all_of(s,
                               [](unsigned char c) { return static_cast<bool>(std::isspace(c)); });
}

void MenuInput::RenderInvalidAndMenu(const MenuNode& current,
                                     std::optional<size_t> default_option) const
{
    renderer_->RenderInvalidChoice();
    renderer_->RenderMenu(current, default_option);
}

std::optional<int> MenuInput::TryReadOption(const std::string& line,
                                            std::optional<size_t> default_option) const
{
    if (default_option.has_value() && (line.empty() || IsBlankLine(line)))
    {
        return static_cast<int>(*default_option);
    }

    try
    {
        return std::stoi(line);
    }
    catch (const std::invalid_argument&)
    {
        return std::nullopt;
    }
    catch (const std::out_of_range&)
    {
        return std::nullopt;
    }
}

std::vector<MenuNode*> MenuInput::GetVisibleChildren(const MenuNode& current) const
{
    const bool is_authenticated = auth_service_ && auth_service_->IsAuthenticated();

    std::vector<MenuNode*> visible_children;
    visible_children.reserve(current.Children().size());
    for (const auto& child : current.Children())
    {
        if (child->IsVisibleTo(is_authenticated))
        {
            visible_children.push_back(child.get());
        }
    }

    return visible_children;
}

MenuNode* MenuInput::ResolveSelection(const MenuNode& current,
                                      std::span<MenuNode* const> visible_children, int option) const
{
    if (option == 0)
    {
        return current.IsRoot() ? nullptr : current.Parent();
    }

    const int index = option - 1;
    if (index < 0)
    {
        return nullptr;
    }

    if (static_cast<size_t>(index) >= visible_children.size())
    {
        return nullptr;
    }

    return visible_children[static_cast<size_t>(index)];
}

MenuNode* MenuInput::ReadSelection(const MenuNode& current,
                                   std::optional<size_t> default_option) const
{
    while (true)
    {
        std::string line = input_->ReadLine();

        const auto option = TryReadOption(line, default_option);
        if (!option.has_value())
        {
            RenderInvalidAndMenu(current, default_option);
            continue;
        }

        const auto visible_children = GetVisibleChildren(current);

        if (MenuNode* selected = ResolveSelection(current, visible_children, *option))
        {
            return selected;
        }

        if (*option == 0)
        {
            return nullptr;
        }

        RenderInvalidAndMenu(current, default_option);
    }
}