#pragma once
#include <memory>
#include <string>
#include <vector>

#include "core/ui/form/form_context.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/input.hpp"
#include "core/ui/menu/menu_input.hpp"
#include "core/ui/menu/menu_renderer.hpp"

namespace form
{

  class MenuFormInputProvider : public FormInputProvider
  {
  public:
    MenuFormInputProvider(std::shared_ptr<Input> input,
                          std::shared_ptr<MenuRenderer> renderer,
                          std::shared_ptr<MenuInput> menu_input);

    std::optional<std::string> ReadField(const Field &field, const FormContext &context) override;

  private:
    std::shared_ptr<Input> input_;
    std::shared_ptr<MenuRenderer> renderer_;
    std::shared_ptr<MenuInput> menu_input_;

    std::optional<std::string> ReadLine() const;
    std::optional<std::string> ReadMenuSelection(const std::string &title,
                                                 const std::vector<std::string> &options) const;
  };

} // namespace form
