#include "core/ui/menu/menu_engine.hpp"

MenuEngine::MenuEngine(std::unique_ptr<MenuNode> root,
                       std::shared_ptr<MenuRenderer> renderer,
                       std::shared_ptr<MenuInput> input,
                       std::shared_ptr<ActionContext> context)
    : root_(std::move(root)),
      renderer_(std::move(renderer)),
      input_(std::move(input)),
      context_(std::move(context))
{
    stack_.push_back(root_.get());
}

void MenuEngine::Run()
{
    while (true)
    {
        auto* current = stack_.back();

        renderer_->RenderMenu(*current);
        MenuNode* selected = input_->ReadSelection(*current);

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
            selected->ExecuteAction(*context_);
        }
        else
        {
            stack_.pop_back();
        }
    }
}
