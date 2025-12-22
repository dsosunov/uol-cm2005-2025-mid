#include "io.hpp"
#include <iostream>
#include <string>

void StandardOutput::Write(const std::string &text)
{
    std::cout << text;
}

void StandardOutput::WriteLine(const std::string &text)
{
    std::cout << text << "\n";
}

std::string StandardInput::ReadLine()
{
    std::string line;
    std::getline(std::cin, line);
    return line;
}
