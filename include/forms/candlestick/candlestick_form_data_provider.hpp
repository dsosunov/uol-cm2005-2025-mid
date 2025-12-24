#pragma once
#include "core/utils/time_utils.hpp"
#include "dto/constants.hpp"
#include "services/trading_service.hpp"

#include <any>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace candlestick
{
class CandlestickFormDataProvider
{
  public:
    using OptionPair = std::pair<std::string, std::any>;
    explicit CandlestickFormDataProvider(std::shared_ptr<services::TradingService> trading_service);
    std::set<std::string, std::less<>> GetAvailableProducts() const;
    std::vector<OptionPair> GetStartDates(dto::Timeframe timeframe) const;
    std::vector<OptionPair> GetEndDates(dto::Timeframe timeframe,
                                        std::optional<utils::TimePoint> start_date) const;

  private:
    std::shared_ptr<services::TradingService> trading_service_;
};
} // namespace candlestick