#include "core/ui/menu/menu_input.hpp"

#include <algorithm>
#include <cctype>
#include <string>

MenuInput::MenuInput(std::shared_ptr<Input> input, std::shared_ptr<MenuRenderer> renderer)
    : input_(std::move(input)), renderer_(std::move(renderer))
{
}

namespace
{
bool IsBlankLine(const std::string& s)
{
    return std::all_of(s.begin(), s.end(),
                       [](unsigned char c) { return static_cast<bool>(std::isspace(c)); });
}
} // namespace

MenuNode* MenuInput::ReadSelection(const MenuNode& current,
                                   std::optional<size_t> default_option) const
{
    while (true)
    {
        std::string line = input_->ReadLine();
        int option = 0;

        if (default_option.has_value() && (line.empty() || IsBlankLine(line)))
        {
            option = static_cast<int>(*default_option);
        }
        else
        {
            try
            {
                option = std::stoi(line);
            }
            catch (const std::invalid_argument&)
            {
                renderer_->RenderInvalidChoice();
                renderer_->RenderMenu(current, default_option);
                continue;
            }
        }

        const size_t childCount = current.Children().size();

        if (option == 0)
        {
            return current.IsRoot() ? nullptr : current.Parent();
        }

        const int index = option - 1;

        if (index < 0 || static_cast<size_t>(index) >= childCount)
        {
            renderer_->RenderInvalidChoice();
            renderer_->RenderMenu(current, default_option);
            continue;
        }

        return current.Children()[static_cast<size_t>(index)].get();
    }
}