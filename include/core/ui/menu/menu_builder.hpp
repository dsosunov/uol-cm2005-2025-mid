#pragma once
#include "core/actions/menu_action.hpp"
#include "core/ui/menu/menu_node.hpp"

#include <any>
#include <memory>
#include <string>

class MenuBuilder
{
  public:
    explicit MenuBuilder(std::string title);
    MenuBuilder& AddLeaf(const std::string& title, std::unique_ptr<MenuAction> action);
    MenuBuilder& AddLeaf(const std::string& title, const std::any& value,
                         std::unique_ptr<MenuAction> action);
    MenuBuilder& AddBranch(const std::string& title);
    MenuBuilder& AddBranch(const std::string& title, const std::any& value);
    MenuBuilder& Parent();
    std::unique_ptr<MenuNode> Build();

  private:
    std::unique_ptr<MenuNode> root_;
    MenuNode* current_;
};