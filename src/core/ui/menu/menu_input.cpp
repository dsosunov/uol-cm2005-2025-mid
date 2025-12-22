#include "core/ui/menu/menu_input.hpp"

#include <string>

MenuInput::MenuInput(std::shared_ptr<Input> input, std::shared_ptr<MenuRenderer> renderer)
    : input_(std::move(input)), renderer_(std::move(renderer)) {}

MenuNode* MenuInput::ReadSelection(const MenuNode& current) const {
  while (true) {
    std::string line = input_->ReadLine();

    int option;
    try {
      option = std::stoi(line);
    } catch (...) {
      renderer_->RenderInvalidChoice();
      renderer_->RenderMenu(current);
      continue;
    }

    const size_t childCount = current.Children().size();

    if (option == 0) {
      return current.IsRoot() ? nullptr : current.Parent();
    }

    const int index = option - 1;
    if (index < 0 || static_cast<size_t>(index) >= childCount) {
      renderer_->RenderInvalidChoice();
      renderer_->RenderMenu(current);
      continue;
    }

    return current.Children()[static_cast<size_t>(index)].get();
  }
}
