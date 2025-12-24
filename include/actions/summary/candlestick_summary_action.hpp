#pragma once
#include "core/actions/form_based_action.hpp"
#include "core/utils/service_result.hpp"
#include "dto/candlestick_query.hpp"
#include "forms/candlestick/candlestick_form.hpp"
#include "services/trading_service.hpp"

#include <memory>

class CandlestickSummaryAction
    : public actions::FormBasedAction<candlestick::CandlestickForm, dto::CandlestickQuery,
                                      utils::ServiceResult<services::CandlestickSummaryData>>
{
  public:
    explicit CandlestickSummaryAction(std::shared_ptr<services::TradingService> trading_service);

  protected:
    candlestick::CandlestickForm CreateForm(ActionContext& context) override;
    utils::ServiceResult<services::CandlestickSummaryData> ExecuteService(
        const dto::CandlestickQuery& data, ActionContext& context) override;
    void DisplayResults(const utils::ServiceResult<services::CandlestickSummaryData>& result,
                        const dto::CandlestickQuery& data, ActionContext& context) override;
    const char* GetOperationName() const override
    {
        return "Candlestick Summary";
    }

  private:
    std::shared_ptr<services::TradingService> trading_service_;
};