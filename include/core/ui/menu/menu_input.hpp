#pragma once
#include "core/ui/io/input.hpp"
#include "core/ui/menu/menu_node.hpp"
#include "core/ui/menu/menu_renderer.hpp"

#include <memory>
#include <optional>

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
    std::shared_ptr<Input> input_;
    std::shared_ptr<MenuRenderer> renderer_;
    std::shared_ptr<services::AuthenticationService> auth_service_;
};