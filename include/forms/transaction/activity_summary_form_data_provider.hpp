#pragma once
#include <memory>
#include <string>
#include <vector>

#include "services/trading_service.hpp"

namespace transaction_forms
{

    /**
     * @brief Data provider for ActivitySummaryForm - bridges service layer and form layer
     *
     * Encapsulates all logic for preparing data needed by ActivitySummaryForm.
     * Form creates ContextualDataSource lambdas that call methods on this provider.
     *
     * Responsibilities:
     * - Knows what data the form needs
     * - Knows how to fetch it from TradingService
     * - Implements business logic for date filtering
     * - Provides clean API for form to consume
     */
    class ActivitySummaryFormDataProvider
    {
    public:
        explicit ActivitySummaryFormDataProvider(std::shared_ptr<services::TradingService> trading_service);

        /**
         * @brief Get start date options based on timeframe
         * @param timeframe "daily", "monthly", or "yearly"
         * @return Vector of date strings appropriate for the timeframe
         */
        std::vector<std::string> GetStartDates(const std::string &timeframe) const;

        /**
         * @brief Get end date options based on timeframe and start date
         * @param timeframe "daily", "monthly", or "yearly"
         * @param start_date Optional start date for filtering (empty if not selected yet)
         * @return Vector of date strings >= start_date (or all if start_date empty)
         */
        std::vector<std::string> GetEndDates(const std::string &timeframe,
                                             const std::string &start_date) const;

    private:
        std::shared_ptr<services::TradingService> trading_service_;

        std::vector<std::string> GetDatesByTimeframe(const std::string &timeframe) const;
    };

} // namespace transaction_forms
