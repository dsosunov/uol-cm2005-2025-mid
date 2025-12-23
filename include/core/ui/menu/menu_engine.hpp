#pragma once
#include <memory>
#include <vector>

#include "core/actions/action_context.hpp"
#include "core/ui/menu/menu_input.hpp"
#include "core/ui/menu/menu_node.hpp"
#include "core/ui/menu/menu_renderer.hpp"

class MenuEngine {
 public:
  MenuEngine(std::unique_ptr<MenuNode> root, std::shared_ptr<MenuRenderer> renderer,
             std::shared_ptr<MenuInput> input, std::shared_ptr<ActionContext> context);

  void Run();

 private:
  std::unique_ptr<MenuNode> root_;
  std::vector<MenuNode*> stack_;
  std::shared_ptr<MenuRenderer> renderer_;
  std::shared_ptr<MenuInput> input_;
  std::shared_ptr<ActionContext> context_;
};
