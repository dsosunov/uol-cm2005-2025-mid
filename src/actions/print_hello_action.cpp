#include "actions/print_hello_action.hpp"
#include <iostream>

void PrintHelloAction::Execute()
{
    std::cout << "Hello!\n";
}
