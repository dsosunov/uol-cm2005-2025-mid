#include "core/ui/menu/menu_engine.hpp"

#include <format>

MenuEngine::MenuEngine(std::unique_ptr<MenuNode> root, std::shared_ptr<MenuRenderer> renderer,
                       std::shared_ptr<MenuInput> input, std::shared_ptr<ActionContext> context)
    : root_(std::move(root)), renderer_(std::move(renderer)), input_(std::move(input)),
      context_(std::move(context))
{
    stack_.push_back(root_.get());
}

void MenuEngine::Run()
{
    while (true)
    {
        const auto* current = stack_.back();

        const auto current_user = context_->auth_service->GetCurrentUser().data.value();
        const bool is_authenticated = context_->auth_service->IsAuthenticated();
        const std::string user_label =
            is_authenticated ? std::format("{} ({})", current_user.username, current_user.full_name)
                             : "Guest";
        const std::string status_line = std::format("Current user: {}", user_label);

        renderer_->RenderMenu(*current, status_line);
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
            renderer_->RenderActionHeader(*selected);
            selected->ExecuteAction(*context_);
        }
        else
        {
            stack_.pop_back();
        }
    }
}