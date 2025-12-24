#include "forms/shared/fields/date_fields.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "dto/activity_summary.hpp"
#include "dto/candlestick_query.hpp"

namespace forms::shared
{

template <typename TDTO>
StartDateField<TDTO>::StartDateField(std::shared_ptr<form::DataSource> data_source,
                                     MemberPtr member_ptr)
    : form::SelectionField("start_date", "Select start date", std::move(data_source),
                           form::DateFieldBinder<TDTO>(member_ptr))
{
}

template <typename TDTO>
EndDateField<TDTO>::EndDateField(std::shared_ptr<form::DataSource> data_source,
                                 MemberPtr member_ptr)
    : form::SelectionField("end_date", "Select end_date", std::move(data_source),
                           form::DateFieldBinder<TDTO>(member_ptr))
{
}

template class StartDateField<dto::CandlestickQuery>;
template class StartDateField<dto::ActivitySummary>;
template class EndDateField<dto::CandlestickQuery>;
template class EndDateField<dto::ActivitySummary>;

} // namespace forms::shared