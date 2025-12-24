#pragma once
#include "core/ui/form/data_source.hpp"
#include "dto/candlestick_query.hpp"
#include "forms/shared/fields/date_fields.hpp"

#include <memory>

namespace candlestick
{
class StartDateField : public forms::shared::StartDateField<dto::CandlestickQuery>
{
  public:
    explicit StartDateField(std::shared_ptr<form::DataSource> data_source);
};
} // namespace candlestick