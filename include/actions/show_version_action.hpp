#pragma once
#include "core/actions/menu_action.hpp"

struct ShowVersionAction final : MenuAction
{
    void Execute() override;
};
