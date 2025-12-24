#pragma once
#include "core/actions/form_based_action.hpp"
#include "dto/transaction_query.hpp"
#include "forms/transaction/currency_form.hpp"
#include "services/trading_service.hpp"
#include "services/transactions_service.hpp"

#include <memory>
#include <vector>

class TransactionShowByCurrencyAction
    : public actions::FormBasedAction<
          transaction_forms::CurrencyForm, dto::TransactionQuery,
          utils::ServiceResult<std::vector<services::WalletTransaction>>>
{
  public:
    explicit TransactionShowByCurrencyAction(
        std::shared_ptr<services::TransactionsService> transactions_service,
        std::shared_ptr<services::TradingService> trading_service);

  protected:
    transaction_forms::CurrencyForm CreateForm(ActionContext& context) override;
    utils::ServiceResult<std::vector<services::WalletTransaction>> ExecuteService(
        const dto::TransactionQuery& data, ActionContext& context) override;
    void DisplayResults(
        const utils::ServiceResult<std::vector<services::WalletTransaction>>& result,
        const dto::TransactionQuery& data, ActionContext& context) override;
    const char* GetOperationName() const override
    {
        return "Transaction Query";
    }

  private:
    std::shared_ptr<services::TransactionsService> transactions_service_;
    std::shared_ptr<services::TradingService> trading_service_;
};