#pragma once
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include "services/trading_service.hpp"
namespace candlestick
{
    class CandlestickFormDataProvider
    {
    public:
        using OptionPair = std::pair<std::string, std::string>;
        explicit CandlestickFormDataProvider(std::shared_ptr<services::TradingService> trading_service);
        std::set<std::string, std::less<>> GetAvailableProducts() const;
        std::vector<OptionPair> GetStartDates(std::string_view timeframe) const;
        std::vector<OptionPair> GetEndDates(std::string_view timeframe,
                                            std::string_view start_date) const;
    private:
        std::shared_ptr<services::TradingService> trading_service_;
        std::vector<OptionPair> GetDatesByTimeframe(std::string_view timeframe) const;
    };
}