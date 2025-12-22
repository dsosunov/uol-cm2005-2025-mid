#include "core/ui/io/standard_input.hpp"

#include <iostream>

std::string StandardInput::ReadLine() {
  std::string line;
  std::getline(std::cin, line);
  return line;
}
