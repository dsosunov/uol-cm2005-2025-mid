#pragma once
#include "io/input.hpp"
#include "menu/menu_node.hpp"
#include "menu/menu_renderer.hpp"

class MenuInput
{
public:
    MenuInput(Input& input, MenuRenderer& renderer);

    MenuNode* ReadSelection(const MenuNode& current) const;

private:
    Input& input_;
    MenuRenderer& renderer_;
};
