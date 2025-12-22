#ifndef FORMS_CANDLESTICK_CANDLESTICK_FORM_HPP_
#define FORMS_CANDLESTICK_CANDLESTICK_FORM_HPP_

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "core/ui/form/form.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"

namespace candlestick {

class CandlestickForm : public form::Form {
 public:
  CandlestickForm(std::shared_ptr<form::FormInputProvider> input_provider,
                  std::shared_ptr<Output> output, const std::set<std::string>& allowed_currencies);

 private:
  static std::vector<std::shared_ptr<form::Field>> SetupFormLayout(
      const std::set<std::string>& allowed_currencies);
};

}  // namespace candlestick

#endif  // FORMS_CANDLESTICK_CANDLESTICK_FORM_HPP_
