#pragma once
#include <memory>
#include "core/actions/menu_action.hpp"
#include "dto/product_query.hpp"
#include "forms/product_query/product_query_form.hpp"
#include "core/ui/io/input.hpp"
#include "core/ui/io/output.hpp"
#include "core/ui/menu/menu_renderer.hpp"
#include "core/ui/menu/menu_input.hpp"

class ProductQueryAction final : public MenuAction {
 public:
  ProductQueryAction(std::shared_ptr<Input> input,
                     std::shared_ptr<Output> output,
                     MenuRenderer& renderer,
                     MenuInput& menu_input);

  void Execute() override;

 private:
  std::shared_ptr<Input> input_;
  std::shared_ptr<Output> output_;
  product_query::ProductQueryForm form_;

  void DisplayResults(const dto::ProductQuery& query);
};
