#include "forms/candlestick/fields/start_date_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"

namespace candlestick {

StartDateField::StartDateField(std::shared_ptr<form::DataSource> data_source)
    : form::SelectionField("start_date", "Select start date", std::move(data_source),
                           form::SimpleFieldBinder<dto::CandlestickQuery, std::string>(
                               &dto::CandlestickQuery::start_date)) {}

}  // namespace candlestick
