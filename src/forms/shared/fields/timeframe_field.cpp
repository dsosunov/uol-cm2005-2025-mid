#include "forms/shared/fields/timeframe_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "core/ui/form/static_data_source.hpp"
#include "dto/activity_summary.hpp"
#include "dto/candlestick_query.hpp"
#include "dto/constants.hpp"

namespace forms::shared
{

template <typename TDTO>
TimeframeField<TDTO>::TimeframeField(MemberPtr member_ptr)
    : form::SelectionField(
          "timeframe", "Select timeframe",
          std::make_shared<form::StaticDataSource>(
              std::vector<form::DataSource::OptionPair>{{"Daily", dto::Timeframe::Daily},
                                                        {"Monthly", dto::Timeframe::Monthly},
                                                        {"Yearly", dto::Timeframe::Yearly}}),
          form::TimeframeFieldBinder<TDTO>(member_ptr), std::optional<size_t>{3})
{
}

template class TimeframeField<dto::CandlestickQuery>;
template class TimeframeField<dto::ActivitySummary>;

} // namespace forms::shared