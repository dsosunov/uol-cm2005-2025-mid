#include "io/standard_output.hpp"
#include <iostream>

void StandardOutput::Write(const std::string &text)
{
    std::cout << text;
}

void StandardOutput::WriteLine(const std::string &text)
{
    std::cout << text << "\n";
}
