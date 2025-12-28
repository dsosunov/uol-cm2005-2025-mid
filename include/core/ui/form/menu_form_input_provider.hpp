#pragma once
#include "core/ui/form/form_context.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/input.hpp"
#include "core/ui/menu/menu_input.hpp"
#include "core/ui/menu/menu_renderer.hpp"

#include <any>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace form
{
class MenuFormInputProvider : public FormInputProvider
{
  public:
    MenuFormInputProvider(std::shared_ptr<Input> input, std::shared_ptr<MenuRenderer> renderer,
                          std::shared_ptr<MenuInput> menu_input);
    std::optional<std::any> ReadField(const Field& field, const FormContext& context) override;

  private:
    std::shared_ptr<Input> input_;
    std::shared_ptr<MenuRenderer> renderer_;
    std::shared_ptr<MenuInput> menu_input_;
    std::optional<std::any> ReadLine() const;
    std::optional<std::any> ReadMenuSelection(
        const std::string& title, const std::vector<std::pair<std::string, std::any>>& option_pairs,
        std::optional<size_t> default_option) const;
};
} // namespace form