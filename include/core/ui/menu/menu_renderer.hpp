#pragma once
#include "core/ui/io/output.hpp"
#include "core/ui/menu/menu_node.hpp"

#include <memory>

class MenuRenderer
{
  public:
    explicit MenuRenderer(std::shared_ptr<Output> output);
    void RenderMenu(const MenuNode& current) const;
    void RenderActionHeader(const MenuNode& node) const;
    void RenderInvalidChoice() const;

  private:
    std::shared_ptr<Output> output_;
    std::string BuildBreadcrumbPath(const MenuNode& current) const;
};