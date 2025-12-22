#pragma once
#include <memory>

#include "core/ui/io/output.hpp"
#include "core/ui/menu/menu_node.hpp"

class MenuRenderer {
 public:
  explicit MenuRenderer(std::shared_ptr<Output> output);

  void RenderMenu(const MenuNode& current) const;
  void RenderInvalidChoice() const;

 private:
  std::shared_ptr<Output> output_;
  void RenderBreadcrumbs(const MenuNode& current) const;
};
