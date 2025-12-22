#pragma once
#include <memory>
#include <vector>
#include "core/ui/menu/menu_node.hpp"
#include "core/ui/menu/menu_renderer.hpp"
#include "core/ui/menu/menu_input.hpp"

class MenuEngine
{
public:
    MenuEngine(std::unique_ptr<MenuNode> root, MenuRenderer renderer, MenuInput input);

    void Run();

private:
    std::unique_ptr<MenuNode> root_;
    std::vector<MenuNode*> stack_;
    MenuRenderer renderer_;
    MenuInput input_;
};
