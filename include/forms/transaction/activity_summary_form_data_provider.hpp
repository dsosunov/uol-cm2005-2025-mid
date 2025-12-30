#pragma once
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include "services/transactions_service.hpp"

#include <any>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace transaction_forms
{
class ActivitySummaryFormDataProvider
{
  public:
    using OptionPair = std::pair<std::string, std::any>;
    explicit ActivitySummaryFormDataProvider(
        std::shared_ptr<services::TransactionsService> transactions_service, int user_id);
    std::vector<OptionPair> GetStartDates(dto::Timeframe timeframe) const;
    std::vector<OptionPair> GetEndDates(dto::Timeframe timeframe,
                                        std::optional<utils::TimePoint> start_date) const;

  private:
    std::shared_ptr<services::TransactionsService> transactions_service_;
    int user_id_ = 0;
};
} // namespace transaction_forms