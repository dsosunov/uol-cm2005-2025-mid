#include "core/ui/io/standard_output.hpp"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

StandardOutput::StandardOutput() {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
#endif
}

void StandardOutput::Write(const std::string& text) { std::cout << text; }

void StandardOutput::WriteLine(const std::string& text) { std::cout << text << "\n"; }
