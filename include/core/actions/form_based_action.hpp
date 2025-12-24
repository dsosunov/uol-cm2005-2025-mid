#pragma once
#include "core/actions/menu_action.hpp"
#include "core/utils/service_result.hpp"

#include <memory>

namespace actions
{
template <typename TForm, typename TData, typename TResult>
class FormBasedAction : public MenuAction
{
  public:
    void Execute(ActionContext& context) override
    {
        TData data;
        TForm form = CreateForm(context);
        if (auto form_result = form.Read(data);
            HandleFormCancellation(form_result, GetOperationName(), context))
        {
            return;
        }
        auto result = ExecuteService(data, context);
        DisplayResults(result, data, context);
    }

  protected:
    virtual TForm CreateForm(ActionContext& context) = 0;
    virtual TResult ExecuteService(const TData& data, ActionContext& context) = 0;
    virtual void DisplayResults(const TResult& result, const TData& data,
                                ActionContext& context) = 0;
    const char* GetOperationName() const override = 0;
};
} // namespace actions
