#pragma once
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include "services/trading_service.hpp"
namespace transaction_forms
{
    class ActivitySummaryFormDataProvider
    {
    public:
        using OptionPair = std::pair<std::string, std::string>;
        explicit ActivitySummaryFormDataProvider(std::shared_ptr<services::TradingService> trading_service);
        std::vector<OptionPair> GetStartDates(std::string_view timeframe) const;
        std::vector<OptionPair> GetEndDates(std::string_view timeframe,
                                            std::string_view start_date) const;
    private:
        std::shared_ptr<services::TradingService> trading_service_;
        std::vector<OptionPair> GetDatesByTimeframe(std::string_view timeframe) const;
    };
}