#include "core/ui/form/menu_form_input_provider.hpp"

#include "core/ui/form/fields/selection_field.hpp"
#include "core/ui/form/fields/text_field.hpp"
#include "core/ui/menu/menu_builder.hpp"

namespace form
{

  MenuFormInputProvider::MenuFormInputProvider(std::shared_ptr<Input> input,
                                               std::shared_ptr<MenuRenderer> renderer,
                                               std::shared_ptr<MenuInput> menu_input)
      : input_(std::move(input)),
        renderer_(std::move(renderer)),
        menu_input_(std::move(menu_input)) {}

  std::optional<std::string> MenuFormInputProvider::ReadField(const Field &field, const FormContext &context)
  {
    // Provider introspects field type and adapts reading mechanism
    if (dynamic_cast<const TextField *>(&field))
    {
      return ReadLine();
    }
    else if (auto *selection_field = dynamic_cast<const SelectionField *>(&field))
    {
      // Get options from field, passing context for data source evaluation
      auto options = selection_field->GetOptions(context);
      return ReadMenuSelection(field.GetPrompt(), options);
    }
    return std::nullopt; // Unknown field type
  }

  std::optional<std::string> MenuFormInputProvider::ReadLine() const
  {
    // Just reads text input - no prompt display, no cancellation check
    return input_->ReadLine();
  }

  std::optional<std::string> MenuFormInputProvider::ReadMenuSelection(
      const std::string &title, const std::vector<std::string> &options) const
  {
    MenuBuilder builder(title);
    for (const auto &option : options)
    {
      builder.AddLeaf(option, nullptr);
    }
    auto menu = builder.Build();

    renderer_->RenderMenu(*menu);
    const MenuNode *selected = menu_input_->ReadSelection(*menu);

    if (!selected || selected == menu.get())
    {
      return std::nullopt; // No selection made
    }

    return selected->Title();
  }

} // namespace form
