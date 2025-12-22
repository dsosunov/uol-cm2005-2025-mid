#pragma once
#include <memory>
#include <string>
#include "menu/menu_node.hpp"
#include "actions/menu_action.hpp"

class MenuBuilder
{
public:
    explicit MenuBuilder(std::string title);

    MenuBuilder& AddLeaf(const std::string& title, std::unique_ptr<MenuAction> action);
    MenuBuilder& AddBranch(const std::string& title);
    MenuBuilder& Parent();
    std::unique_ptr<MenuNode> Build();

private:
    std::unique_ptr<MenuNode> root_;
    MenuNode* current_;
};
