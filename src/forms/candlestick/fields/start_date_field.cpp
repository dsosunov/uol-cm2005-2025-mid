#include "forms/candlestick/fields/start_date_field.hpp"
#include "forms/shared/fields/date_fields.hpp"
namespace candlestick
{
    StartDateField::StartDateField(std::shared_ptr<form::DataSource> data_source)
        : forms::shared::StartDateField<dto::CandlestickQuery>(
              std::move(data_source), &dto::CandlestickQuery::start_date) {}
}