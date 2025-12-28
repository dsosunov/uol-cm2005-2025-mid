#include "core/ui/form/menu_form_input_provider.hpp"

#include "core/ui/form/fields/selection_field.hpp"
#include "core/ui/form/fields/text_field.hpp"
#include "core/ui/menu/menu_builder.hpp"

namespace form
{
MenuFormInputProvider::MenuFormInputProvider(std::shared_ptr<Input> input,
                                             std::shared_ptr<MenuRenderer> renderer,
                                             std::shared_ptr<MenuInput> menu_input)
    : input_(std::move(input)), renderer_(std::move(renderer)), menu_input_(std::move(menu_input))
{
}

std::optional<std::any> MenuFormInputProvider::ReadField(const Field& field,
                                                         const FormContext& context)
{
    if (dynamic_cast<const TextField*>(&field))
    {
        return ReadLine();
    }
    else if (auto* selection_field = dynamic_cast<const SelectionField*>(&field))
    {
        auto option_pairs = selection_field->GetOptions(context);
        std::optional<size_t> default_option = selection_field->GetDefaultIndex();
        if (!default_option.has_value() && !option_pairs.empty())
        {
            default_option = 1;
        }

        if (default_option.has_value() &&
            (*default_option < 1 || *default_option > option_pairs.size()))
        {
            default_option = option_pairs.empty() ? std::nullopt : std::optional<size_t>{1};
        }

        return ReadMenuSelection(field.GetPrompt(), option_pairs, default_option);
    }
    return std::nullopt;
}

std::optional<std::any> MenuFormInputProvider::ReadLine() const
{
    return input_->ReadLine();
}

std::optional<std::any> MenuFormInputProvider::ReadMenuSelection(
    const std::string& title, const std::vector<std::pair<std::string, std::any>>& option_pairs,
    std::optional<size_t> default_option) const
{
    MenuBuilder builder(title);
    for (const auto& [display_title, value] : option_pairs)
    {
        builder.AddLeaf(display_title, value, nullptr);
    }

    auto menu = builder.Build();
    renderer_->RenderMenu(*menu, default_option);
    const MenuNode* selected = menu_input_->ReadSelection(*menu, default_option);

    if (!selected || selected == menu.get())
    {
        return std::nullopt;
    }

    return selected->Value();
}
} // namespace form