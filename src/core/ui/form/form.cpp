#include "core/ui/form/form.hpp"

namespace form
{

  Form::Form(std::vector<std::shared_ptr<Field>> fields,
             std::shared_ptr<FormInputProvider> input_provider, std::shared_ptr<Output> output)
      : fields_(std::move(fields)),
        input_provider_(std::move(input_provider)),
        output_(std::move(output)) {}

} // namespace form
