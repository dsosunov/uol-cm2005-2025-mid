#pragma once
#include "io/output.hpp"
#include "menu/menu_node.hpp"

class MenuRenderer
{
public:
    explicit MenuRenderer(Output& output);

    void RenderMenu(const MenuNode& current) const;
    void RenderInvalidChoice() const;

private:
    Output& output_;
    void RenderBreadcrumbs(const MenuNode& current) const;
};
