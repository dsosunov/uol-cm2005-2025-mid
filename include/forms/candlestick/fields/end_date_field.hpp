#pragma once
#include "core/ui/form/data_source.hpp"
#include "dto/candlestick_query.hpp"
#include "forms/shared/fields/date_fields.hpp"

#include <memory>

namespace candlestick
{
class EndDateField : public forms::shared::EndDateField<dto::CandlestickQuery>
{
  public:
    explicit EndDateField(std::shared_ptr<form::DataSource> data_source);
};
} // namespace candlestick