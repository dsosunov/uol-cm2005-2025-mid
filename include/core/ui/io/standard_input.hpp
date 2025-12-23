#pragma once
#include <string>
#include "core/ui/io/input.hpp"
class StandardInput : public Input {
 public:
  std::string ReadLine() override;
};