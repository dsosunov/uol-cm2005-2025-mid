#include "forms/candlestick/fields/start_date_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"

namespace candlestick
{

StartDateField::StartDateField(std::shared_ptr<form::DataSource> data_source)
    : form::TextField("start_date", "Enter start date",
                      form::SimpleFieldBinder<dto::CandlestickQuery, std::string>(
                          &dto::CandlestickQuery::start_date)) {}

}  // namespace candlestick
