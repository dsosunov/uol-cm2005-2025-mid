#include "forms/candlestick/fields/start_date_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"

namespace candlestick
{

    StartDateField::StartDateField(std::shared_ptr<form::DataSource> data_source)
        : form::SelectionField("start_date", "Select start date", std::move(data_source),
                               form::DateFieldBinder<dto::CandlestickQuery>(
                                   &dto::CandlestickQuery::start_date)) {}

}
