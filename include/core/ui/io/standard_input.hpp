#pragma once
#include "core/ui/io/input.hpp"

#include <string>

class StandardInput : public Input
{
  public:
    std::string ReadLine() override;
};