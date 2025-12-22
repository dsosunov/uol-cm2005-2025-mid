#include "core/ui/form/form.hpp"
#include "core/ui/menu/menu_builder.hpp"

namespace form
{

Form::Form(std::vector<std::shared_ptr<Field>> fields,
           std::shared_ptr<Input> input,
           std::shared_ptr<Output> output,
           MenuRenderer& renderer,
           MenuInput& menu_input)
    : fields_(std::move(fields)),
      input_(std::move(input)),
      output_(std::move(output)),
      renderer_(renderer),
      menu_input_(menu_input) {}

bool Form::IsCancelKeyword(const std::string& value) const {
  return value == "cancel" || value == "Cancel" || value == "CANCEL";
}

}  // namespace form
