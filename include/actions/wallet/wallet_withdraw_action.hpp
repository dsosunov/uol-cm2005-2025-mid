#pragma once
#include "core/actions/form_based_action.hpp"
#include "services/wallet_service.hpp"
#include "services/trading_service.hpp"
#include "dto/wallet_operation.hpp"
#include "forms/wallet/wallet_operation_form.hpp"
#include <memory>
class WalletWithdrawAction : public actions::FormBasedAction<wallet_forms::WalletOperationForm, dto::WalletOperation, utils::ServiceResult<double>>
{
public:
  explicit WalletWithdrawAction(std::shared_ptr<services::WalletService> wallet_service,
                                std::shared_ptr<services::TradingService> trading_service);

protected:
  wallet_forms::WalletOperationForm CreateForm(ActionContext &context) override;
  utils::ServiceResult<double> ExecuteService(const dto::WalletOperation &data, ActionContext &context) override;
  void DisplayResults(const utils::ServiceResult<double> &result, const dto::WalletOperation &data, ActionContext &context) override;
  const char *GetOperationName() const override { return "Withdrawal"; }

private:
  std::shared_ptr<services::WalletService> wallet_service_;
  std::shared_ptr<services::TradingService> trading_service_;
};