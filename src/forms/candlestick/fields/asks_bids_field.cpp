#include "forms/candlestick/fields/asks_bids_field.hpp"
#include "core/ui/form/binders/field_binders.hpp"

namespace candlestick
{

AsksBidsField::AsksBidsField()
    : form::SelectionField(
          "asks_bids",
          "Select data type",
          std::vector<std::string>{"asks", "bids"},
          form::SimpleFieldBinder<dto::CandlestickQuery, std::string>(
              &dto::CandlestickQuery::asks_bids)) {}

}  // namespace candlestick
