#pragma once
#include <memory>
#include <string>
#include <vector>

#include "core/actions/menu_action.hpp"
#include "core/actions/action_context.hpp"

class MenuNode
{
public:
    explicit MenuNode(std::string title, std::unique_ptr<MenuAction> action = nullptr);

    const std::string& Title() const;

    MenuNode& AddChild(std::unique_ptr<MenuNode> child);

    bool HasAction() const;
    bool HasChildren() const;
    bool IsRoot() const;
    MenuNode* Parent() const;

    void ExecuteAction(ActionContext& context) const;

    const std::vector<std::unique_ptr<MenuNode>>& Children() const;

private:
    std::string title_;
    std::unique_ptr<MenuAction> action_;
    std::vector<std::unique_ptr<MenuNode>> children_;
    MenuNode* parent_;
};
