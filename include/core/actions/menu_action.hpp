#pragma once

struct MenuAction
{
    virtual ~MenuAction() = default;
    virtual void Execute() = 0;
};
