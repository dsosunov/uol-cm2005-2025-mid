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
std::optional<std::string> MenuFormInputProvider::ReadField(const Field& field,
                                                            const FormContext& context)
{
    if (dynamic_cast<const TextField*>(&field))
    {
        return ReadLine();
    }
    else if (auto* selection_field = dynamic_cast<const SelectionField*>(&field))
    {
        auto option_pairs = selection_field->GetOptions(context);
        return ReadMenuSelection(field.GetPrompt(), option_pairs);
    }
    return std::nullopt;
}
std::optional<std::string> MenuFormInputProvider::ReadLine() const
{
    return input_->ReadLine();
}
std::optional<std::string> MenuFormInputProvider::ReadMenuSelection(
    const std::string& title,
    const std::vector<std::pair<std::string, std::string>>& option_pairs) const
{
    MenuBuilder builder(title);
    for (const auto& [display_title, value] : option_pairs)
    {
        builder.AddLeaf(display_title, value, nullptr);
    }
    auto menu = builder.Build();
    renderer_->RenderMenu(*menu);
    const MenuNode* selected = menu_input_->ReadSelection(*menu);
    if (!selected || selected == menu.get())
    {
        return std::nullopt;
    }
    return selected->Value();
}
} // namespace form