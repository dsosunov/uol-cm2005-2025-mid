#include "actions/show_version_action.hpp"
#include <iostream>

void ShowVersionAction::Execute()
{
    std::cout << "Version: 1.0.0\n";
}
