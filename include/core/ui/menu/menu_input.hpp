#pragma once
#include "core/ui/io/input.hpp"
#include "core/ui/menu/menu_node.hpp"
#include "core/ui/menu/menu_renderer.hpp"

#include <memory>

class MenuInput
{
  public:
    MenuInput(std::shared_ptr<Input> input, std::shared_ptr<MenuRenderer> renderer);
    MenuNode* ReadSelection(const MenuNode& current) const;

  private:
    std::shared_ptr<Input> input_;
    std::shared_ptr<MenuRenderer> renderer_;
};