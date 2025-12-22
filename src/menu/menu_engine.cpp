#include "menu/menu_engine.hpp"

MenuEngine::MenuEngine(std::unique_ptr<MenuNode> root, MenuRenderer renderer, MenuInput input)
    : root_(std::move(root)), renderer_(std::move(renderer)), input_(std::move(input))
{
    stack_.push_back(root_.get());
}

void MenuEngine::Run()
{
    while (true)
    {
        auto* current = stack_.back();

        renderer_.RenderMenu(*current);
        MenuNode* selected = input_.ReadSelection(*current);

        if (!selected)
        {
            return;
        }

        if (selected->HasChildren())
        {
            stack_.push_back(selected);
        }
        else if (selected->HasAction())
        {
            selected->ExecuteAction();
        }
        else
        {
            stack_.pop_back();
        }
    }
}
