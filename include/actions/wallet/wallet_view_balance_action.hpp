#pragma once
#include "core/actions/form_based_action.hpp"
#include "dto/empty_request.hpp"
#include "forms/shared/empty_form.hpp"
#include "services/wallet_service.hpp"

#include <map>
#include <memory>

class WalletViewBalanceAction
    : public actions::FormBasedAction<
          shared_forms::EmptyForm, EmptyRequest,
          utils::ServiceResult<std::map<std::string, double, std::less<>>>>
{
  public:
    explicit WalletViewBalanceAction(std::shared_ptr<services::WalletService> wallet_service);

  protected:
    shared_forms::EmptyForm CreateForm(ActionContext& context) override;
    utils::ServiceResult<std::map<std::string, double, std::less<>>> ExecuteService(
        const EmptyRequest& data, ActionContext& context) override;
    void DisplayResults(
        const utils::ServiceResult<std::map<std::string, double, std::less<>>>& result,
        const EmptyRequest& data, ActionContext& context) override;
    const char* GetOperationName() const override
    {
        return "View Balances";
    }

  private:
    std::shared_ptr<services::WalletService> wallet_service_;
};