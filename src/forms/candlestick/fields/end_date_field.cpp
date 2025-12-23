#include "forms/candlestick/fields/end_date_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"

namespace candlestick
{

    EndDateField::EndDateField(std::shared_ptr<form::DataSource> data_source)
        : form::SelectionField("end_date", "Select end date", std::move(data_source),
                               form::DateFieldBinder<dto::CandlestickQuery>(
                                   &dto::CandlestickQuery::end_date)) {}

}
