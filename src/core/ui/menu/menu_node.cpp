#include "core/ui/menu/menu_node.hpp"

#include "core/actions/menu_action.hpp"

MenuNode::MenuNode(std::string title, std::unique_ptr<MenuAction> action, std::string value)
    : title_(std::move(title)), value_(std::move(value)), action_(std::move(action))
{
}

const std::string& MenuNode::Title() const
{
    return title_;
}

const std::string& MenuNode::Value() const
{
    return value_;
}

MenuNode& MenuNode::AddChild(std::unique_ptr<MenuNode> child)
{
    child->parent_ = this;
    children_.push_back(std::move(child));

    return *this;
}

bool MenuNode::HasAction() const
{
    return action_ != nullptr;
}

bool MenuNode::HasChildren() const
{
    return !children_.empty();
}

bool MenuNode::IsRoot() const
{
    return parent_ == nullptr;
}

MenuNode* MenuNode::Parent() const
{
    return parent_;
}

void MenuNode::ExecuteAction(ActionContext& context) const
{
    if (action_)
        action_->Execute(context);
}

const std::vector<std::unique_ptr<MenuNode>>& MenuNode::Children() const
{
    return children_;
}