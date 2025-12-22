#include "actions.hpp"
#include <iostream>

void PrintHelloAction::Execute()
{
    std::cout << "Hello!\n";
}

void ShowVersionAction::Execute()
{
    std::cout << "Version: 1.0.0\n";
}
