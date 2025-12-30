#pragma once
#include "core/ui/io/input.hpp"
#include "core/ui/menu/menu_node.hpp"
#include "core/ui/menu/menu_renderer.hpp"

#include <memory>
#include <optional>
#include <span>
#include <string_view>
#include <vector>

namespace services
{
class AuthenticationService;
}

class MenuInput
{
  public:
    MenuInput(std::shared_ptr<Input> input, std::shared_ptr<MenuRenderer> renderer,
              std::shared_ptr<services::AuthenticationService> auth_service);
    MenuNode* ReadSelection(const MenuNode& current,
                            std::optional<size_t> default_option = std::nullopt) const;

  private:
    static bool IsBlankLine(std::string_view s);
    void RenderInvalidAndMenu(const MenuNode& current, std::optional<size_t> default_option) const;
    std::optional<int> TryReadOption(const std::string& line,
                                     std::optional<size_t> default_option) const;
    std::vector<MenuNode*> GetVisibleChildren(const MenuNode& current) const;
    MenuNode* ResolveSelection(const MenuNode& current, std::span<MenuNode* const> visible_children,
                               int option) const;

    std::shared_ptr<Input> input_;
    std::shared_ptr<MenuRenderer> renderer_;
    std::shared_ptr<services::AuthenticationService> auth_service_;
};