#pragma once
#include <memory>
#include "core/actions/menu_action.hpp"
#include "dto/product_query.hpp"

class ProductQueryAction final : public MenuAction {
 public:
  ProductQueryAction() = default;

  void Execute(ActionContext& context) override;

 private:
  void DisplayResults(const dto::ProductQuery& query, ActionContext& context);
};
