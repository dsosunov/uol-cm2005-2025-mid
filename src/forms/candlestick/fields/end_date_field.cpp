#include "forms/candlestick/fields/end_date_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"

namespace candlestick
{

EndDateField::EndDateField(std::shared_ptr<form::DataSource> data_source)
    : form::TextField("end_date", "Enter end date",
                      form::SimpleFieldBinder<dto::CandlestickQuery, std::string>(
                          &dto::CandlestickQuery::end_date)) {}

}  // namespace candlestick
