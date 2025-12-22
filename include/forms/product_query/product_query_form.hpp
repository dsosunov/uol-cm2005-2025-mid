#pragma once
#include <memory>
#include "core/ui/form/form.hpp"
#include "core/ui/form/data_source.hpp"
#include "core/ui/io/input.hpp"
#include "core/ui/io/output.hpp"
#include "core/ui/menu/menu_renderer.hpp"
#include "core/ui/menu/menu_input.hpp"

namespace product_query
{

class ProductQueryForm : public form::Form
{
public:
  ProductQueryForm(std::shared_ptr<Input> input,
                   std::shared_ptr<Output> output,
                   MenuRenderer& renderer,
                   MenuInput& menu_input,
                   std::shared_ptr<form::DataSource> start_date_source,
                   std::shared_ptr<form::DataSource> end_date_source);
};

}  // namespace product_query
