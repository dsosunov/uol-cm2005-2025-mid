#include "forms/candlestick/fields/end_date_field.hpp"

#include "forms/shared/fields/date_fields.hpp"

namespace candlestick
{

EndDateField::EndDateField(std::shared_ptr<form::DataSource> data_source)
    : forms::shared::EndDateField<dto::CandlestickQuery>(std::move(data_source),
                                                         &dto::CandlestickQuery::end_date)
{
}

} // namespace candlestick