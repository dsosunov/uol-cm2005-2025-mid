#pragma once
#include "core/actions/form_based_action.hpp"
#include "dto/empty_request.hpp"
#include "forms/shared/empty_form.hpp"
#include "services/user_service.hpp"

#include <memory>

class UserLogoffAction : public actions::FormBasedAction<shared_forms::EmptyForm, EmptyRequest,
                                                         utils::ServiceResult<void>>
{
  public:
    explicit UserLogoffAction(std::shared_ptr<services::UserService> user_service);

  protected:
    shared_forms::EmptyForm CreateForm(ActionContext& context) override;
    utils::ServiceResult<void> ExecuteService(const EmptyRequest& data,
                                              ActionContext& context) override;
    void DisplayResults(const utils::ServiceResult<void>& result, const EmptyRequest& data,
                        ActionContext& context) override;
    const char* GetOperationName() const override
    {
        return "Logoff";
    }

  private:
    std::shared_ptr<services::UserService> user_service_;
};
