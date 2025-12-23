#pragma once
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "services/trading_service.hpp"

namespace candlestick
{

    /**
     * @brief Data provider for CandlestickForm - bridges service layer and form layer
     *
     * Encapsulates all logic for preparing data needed by CandlestickForm.
     * Form creates ContextualDataSource lambdas that call methods on this provider.
     *
     * Responsibilities:
     * - Knows what data the form needs
     * - Knows how to fetch it from TradingService
     * - Implements business logic for date filtering
     * - Provides clean API for form to consume
     */
    class CandlestickFormDataProvider
    {
    public:
        explicit CandlestickFormDataProvider(std::shared_ptr<services::TradingService> trading_service);

        /**
         * @brief Get available currencies for currency pair selection
         * @return Set of currency codes
         */
        std::set<std::string, std::less<>> GetAvailableCurrencies() const;

        /**
         * @brief Get start date options based on timeframe
         * @param timeframe "daily", "monthly", or "yearly"
         * @return Vector of date strings appropriate for the timeframe
         */
        std::vector<std::string> GetStartDates(std::string_view timeframe) const;

        /**
         * @brief Get end date options based on timeframe and start date
         * @param timeframe "daily", "monthly", or "yearly"
         * @param start_date Optional start date for filtering (empty if not selected yet)
         * @return Vector of date strings >= start_date (or all if start_date empty)
         */
        std::vector<std::string> GetEndDates(std::string_view timeframe,
                                             std::string_view start_date) const;

    private:
        std::shared_ptr<services::TradingService> trading_service_;

        std::vector<std::string> GetDatesByTimeframe(std::string_view timeframe) const;
    };

} // namespace candlestick
