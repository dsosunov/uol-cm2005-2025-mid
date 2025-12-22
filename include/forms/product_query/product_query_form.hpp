#ifndef FORMS_PRODUCT_QUERY_PRODUCT_QUERY_FORM_HPP_
#define FORMS_PRODUCT_QUERY_PRODUCT_QUERY_FORM_HPP_

#include "core/ui/form/form.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace product_query {

class ProductQueryForm : public form::Form {
 public:
  ProductQueryForm(std::shared_ptr<form::FormInputProvider> input_provider,
                   std::shared_ptr<Output> output,
                   const std::set<std::string>& allowed_currencies);

 private:
  static std::vector<std::shared_ptr<form::Field>> SetupFormLayout(
      const std::set<std::string>& allowed_currencies);
};

}  // namespace product_query

#endif  // FORMS_PRODUCT_QUERY_PRODUCT_QUERY_FORM_HPP_
