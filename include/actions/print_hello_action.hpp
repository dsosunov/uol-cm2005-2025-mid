#pragma once
#include "core/actions/menu_action.hpp"

struct PrintHelloAction final : MenuAction
{
    void Execute() override;
};
