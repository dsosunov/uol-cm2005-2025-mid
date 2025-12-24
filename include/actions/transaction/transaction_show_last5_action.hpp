#pragma once
#include "core/actions/form_based_action.hpp"
#include "dto/empty_request.hpp"
#include "forms/shared/empty_form.hpp"
#include "services/transactions_service.hpp"

#include <memory>
#include <vector>

class TransactionShowLast5Action
    : public actions::FormBasedAction<shared_forms::EmptyForm, EmptyRequest,
                                      utils::ServiceResult<std::vector<services::Transaction>>>
{
  public:
    explicit TransactionShowLast5Action(
        std::shared_ptr<services::TransactionsService> transactions_service);

  protected:
    shared_forms::EmptyForm CreateForm(ActionContext& context) override;
    utils::ServiceResult<std::vector<services::Transaction>> ExecuteService(
        const EmptyRequest& data, ActionContext& context) override;
    void DisplayResults(const utils::ServiceResult<std::vector<services::Transaction>>& result,
                        const EmptyRequest& data, ActionContext& context) override;
    const char* GetOperationName() const override
    {
        return "Last 5 Transactions";
    }

  private:
    std::shared_ptr<services::TransactionsService> transactions_service_;
};