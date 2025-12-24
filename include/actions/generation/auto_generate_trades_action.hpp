#pragma once
#include "core/actions/form_based_action.hpp"
#include "core/utils/service_result.hpp"
#include "dto/generation_request.hpp"
#include "forms/generation/generation_confirmation_form.hpp"
#include "services/trading_service.hpp"

#include <memory>

class AutoGenerateTradesAction
    : public actions::FormBasedAction<generation_forms::GenerationConfirmationForm,
                                      dto::GenerationRequest,
                                      utils::ServiceResult<services::GenerationData>>
{
  public:
    explicit AutoGenerateTradesAction(std::shared_ptr<services::TradingService> trading_service);

  protected:
    generation_forms::GenerationConfirmationForm CreateForm(ActionContext& context) override;
    utils::ServiceResult<services::GenerationData> ExecuteService(
        const dto::GenerationRequest& data, ActionContext& context) override;
    void DisplayResults(const utils::ServiceResult<services::GenerationData>& result,
                        const dto::GenerationRequest& data, ActionContext& context) override;
    const char* GetOperationName() const override
    {
        return "Trade Generation";
    }

  private:
    std::shared_ptr<services::TradingService> trading_service_;
};