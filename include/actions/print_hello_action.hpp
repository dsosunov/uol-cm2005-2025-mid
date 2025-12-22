#pragma once
#include "actions/menu_action.hpp"

struct PrintHelloAction final : MenuAction
{
    void Execute() override;
};
