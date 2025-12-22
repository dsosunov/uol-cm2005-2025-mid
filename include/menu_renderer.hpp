#pragma once
#include "io.hpp"
#include "menu_node.hpp"

class MenuRenderer
{
public:
    explicit MenuRenderer(IOutput& output);

    void RenderMenu(const MenuNode& current) const;
    void RenderInvalidChoice() const;

private:
    IOutput& _output;
    void RenderBreadcrumbs(const MenuNode& current) const;
};
