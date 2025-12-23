#include "forms/candlestick/fields/timeframe_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "core/ui/form/static_data_source.hpp"
#include "dto/constants.hpp"

namespace candlestick
{

    TimeframeField::TimeframeField()
        : form::SelectionField("timeframe", "Select timeframe",
                               std::make_shared<form::StaticDataSource>(
                                   std::vector<form::DataSource::OptionPair>{
                                       {"Daily", "daily"},
                                       {"Monthly", "monthly"},
                                       {"Yearly", "yearly"}}),
                               form::TimeframeFieldBinder<dto::CandlestickQuery>(
                                   &dto::CandlestickQuery::timeframe)) {}

}
