#include "core/ui/form/menu_form_input_provider.hpp"

#include "core/ui/menu/menu_builder.hpp"

namespace form {

MenuFormInputProvider::MenuFormInputProvider(std::shared_ptr<Input> input,
                                             std::shared_ptr<Output> output,
                                             std::shared_ptr<MenuRenderer> renderer,
                                             std::shared_ptr<MenuInput> menu_input)
    : input_(std::move(input)),
      output_(std::move(output)),
      renderer_(std::move(renderer)),
      menu_input_(std::move(menu_input)) {}

std::string MenuFormInputProvider::ReadText(const std::string& prompt) {
  output_->Write(prompt + ": ");
  return input_->ReadLine();
}

std::optional<std::string> MenuFormInputProvider::ReadSelection(
    const std::string& title, const std::vector<std::string>& options) {
  MenuBuilder builder(title);
  for (const auto& option : options) {
    builder.AddLeaf(option, nullptr);
  }
  auto menu = builder.Build();

  renderer_->RenderMenu(*menu);
  MenuNode* selected = menu_input_->ReadSelection(*menu);

  if (!selected || selected == menu.get()) {
    return std::nullopt;
  }

  return selected->Title();
}

}  // namespace form
