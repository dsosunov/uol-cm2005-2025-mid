#ifndef FORMS_PRODUCT_QUERY_PRODUCT_QUERY_FORM_FACTORY_HPP_
#define FORMS_PRODUCT_QUERY_PRODUCT_QUERY_FORM_FACTORY_HPP_

#include "core/ui/form/form.hpp"
#include "core/ui/form/form_builder.hpp"
#include "core/actions/action_context.hpp"
#include <memory>
#include <set>
#include <string>

namespace product_query {

class ProductQueryFormFactory {
 public:
  explicit ProductQueryFormFactory(const std::set<std::string>& allowed_currencies);

  std::unique_ptr<form::Form> CreateForm(ActionContext& context);

 private:
  std::set<std::string> allowed_currencies_;

  void SetupFormLayout(form::FormBuilder& builder);
};

}  // namespace product_query

#endif  // FORMS_PRODUCT_QUERY_PRODUCT_QUERY_FORM_FACTORY_HPP_
