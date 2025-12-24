#include "core/ui/menu/menu_builder.hpp"
MenuBuilder::MenuBuilder(std::string title)
    : root_(std::make_unique<MenuNode>(title, nullptr, title)), current_(root_.get())
{
}

MenuBuilder& MenuBuilder::AddLeaf(const std::string& title, std::unique_ptr<MenuAction> action)
{
    current_->AddChild(std::make_unique<MenuNode>(title, std::move(action), title));
    return *this;
}

MenuBuilder& MenuBuilder::AddLeaf(const std::string& title, const std::any& value,
                                  std::unique_ptr<MenuAction> action)
{
    current_->AddChild(std::make_unique<MenuNode>(title, std::move(action), value));
    return *this;
}

MenuBuilder& MenuBuilder::AddBranch(const std::string& title)
{
    auto branch = std::make_unique<MenuNode>(title, nullptr, title);
    MenuNode* branchPtr = branch.get();

    current_->AddChild(std::move(branch));
    current_ = branchPtr;

    return *this;
}

MenuBuilder& MenuBuilder::AddBranch(const std::string& title, const std::any& value)
{
    auto branch = std::make_unique<MenuNode>(title, nullptr, value);
    MenuNode* branchPtr = branch.get();

    current_->AddChild(std::move(branch));
    current_ = branchPtr;

    return *this;
}

MenuBuilder& MenuBuilder::Parent()
{
    if (current_->Parent())
    {
        current_ = current_->Parent();
    }

    return *this;
}

std::unique_ptr<MenuNode> MenuBuilder::Build()
{
    return std::move(root_);
}