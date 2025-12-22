#pragma once
#include "core/ui/io/output.hpp"
#include "core/ui/menu/menu_node.hpp"

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
