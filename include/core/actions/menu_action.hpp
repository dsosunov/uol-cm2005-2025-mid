#pragma once
#include "core/actions/action_context.hpp"

struct MenuAction
{
    virtual ~MenuAction() = default;
    virtual void Execute(ActionContext& context) = 0;
};
