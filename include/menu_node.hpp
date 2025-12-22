#pragma once
#include <memory>
#include <string>
#include <vector>

struct IMenuAction;

class MenuNode
{
public:
    explicit MenuNode(std::string title, std::unique_ptr<IMenuAction> action = nullptr);

    const std::string& Title() const;

    MenuNode& AddChild(std::unique_ptr<MenuNode> child);

    bool HasAction() const;
    bool HasChildren() const;
    bool IsRoot() const;
    MenuNode* Parent() const;

    void ExecuteAction() const;

    const std::vector<std::unique_ptr<MenuNode>>& Children() const;

private:
    std::string _title;
    std::unique_ptr<IMenuAction> _action;
    std::vector<std::unique_ptr<MenuNode>> _children;
    MenuNode* _parent;
};
