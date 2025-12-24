#include "forms/candlestick/fields/asks_bids_field.hpp"

#include "core/ui/form/binders/field_binders.hpp"
#include "core/ui/form/static_data_source.hpp"
#include "dto/constants.hpp"

namespace candlestick
{

AsksBidsField::AsksBidsField()
    : form::SelectionField(
          "order_type", "Select data type",
          std::make_shared<form::StaticDataSource>(
              std::vector<form::DataSource::OptionPair>{{"Asks", "asks"}, {"Bids", "bids"}}),
          form::OrderTypeFieldBinder<dto::CandlestickQuery>(&dto::CandlestickQuery::order_type))
{
}

} // namespace candlestick