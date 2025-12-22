#pragma once
#include "actions/menu_action.hpp"

struct ShowVersionAction final : MenuAction
{
    void Execute() override;
};
