#pragma once
#include "core/ui/io/input.hpp"
#include "core/ui/menu/menu_node.hpp"
#include "core/ui/menu/menu_renderer.hpp"

class MenuInput
{
public:
    MenuInput(Input& input, MenuRenderer& renderer);

    MenuNode* ReadSelection(const MenuNode& current) const;

private:
    Input& input_;
    MenuRenderer& renderer_;
};
