#pragma once
#include "core/actions/form_based_action.hpp"
#include "dto/activity_summary.hpp"
#include "forms/transaction/activity_summary_form.hpp"
#include "services/transactions_service.hpp"

#include <memory>

class TransactionActivitySummaryAction
    : public actions::FormBasedAction<transaction_forms::ActivitySummaryForm, dto::ActivitySummary,
                                      utils::ServiceResult<services::ActivityStats>>
{
  public:
    explicit TransactionActivitySummaryAction(
        std::shared_ptr<services::TransactionsService> transactions_service);

  protected:
    transaction_forms::ActivitySummaryForm CreateForm(ActionContext& context) override;
    utils::ServiceResult<services::ActivityStats> ExecuteService(const dto::ActivitySummary& data,
                                                                 ActionContext& context) override;
    void DisplayResults(const utils::ServiceResult<services::ActivityStats>& result,
                        const dto::ActivitySummary& data, ActionContext& context) override;
    const char* GetOperationName() const override
    {
        return "Activity Summary";
    }

  private:
    std::shared_ptr<services::TransactionsService> transactions_service_;
};