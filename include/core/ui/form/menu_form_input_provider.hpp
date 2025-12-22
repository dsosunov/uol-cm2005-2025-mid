#pragma once
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/input.hpp"
#include "core/ui/io/output.hpp"
#include "core/ui/menu/menu_renderer.hpp"
#include "core/ui/menu/menu_input.hpp"

namespace form {

/// Implementation of FormInputProvider that uses the menu system
/// for rendering selections and standard input/output for text.
class MenuFormInputProvider : public FormInputProvider {
public:
    MenuFormInputProvider(std::shared_ptr<Input> input,
                         std::shared_ptr<Output> output,
                         std::shared_ptr<MenuRenderer> renderer,
                         std::shared_ptr<MenuInput> menu_input);

    std::string ReadText(const std::string& prompt) override;

    std::optional<std::string> ReadSelection(
        const std::string& title,
        const std::vector<std::string>& options) override;

private:
    std::shared_ptr<Input> input_;
    std::shared_ptr<Output> output_;
    std::shared_ptr<MenuRenderer> renderer_;
    std::shared_ptr<MenuInput> menu_input_;
};

} // namespace form
