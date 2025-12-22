#include "menu_node.hpp"
#include "actions.hpp"

MenuNode::MenuNode(std::string title, std::unique_ptr<IMenuAction> action)
    : _title(std::move(title)), _action(std::move(action)), _parent(nullptr)
{
}

const std::string &MenuNode::Title() const { return _title; }

MenuNode &MenuNode::AddChild(std::unique_ptr<MenuNode> child)
{
    child->_parent = this;
    _children.push_back(std::move(child));
    return *this;
}

bool MenuNode::HasAction() const { return _action != nullptr; }
bool MenuNode::HasChildren() const { return !_children.empty(); }
bool MenuNode::IsRoot() const { return _parent == nullptr; }
MenuNode *MenuNode::Parent() const { return _parent; }

void MenuNode::ExecuteAction() const
{
    if (_action)
        _action->Execute();
}

const std::vector<std::unique_ptr<MenuNode>> &MenuNode::Children() const { return _children; }
