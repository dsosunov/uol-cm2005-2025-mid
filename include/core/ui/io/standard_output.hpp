#pragma once
#include "core/ui/io/output.hpp"

#include <string_view>

class StandardOutput : public Output
{
  public:
    StandardOutput();
    void Write(std::string_view text) override;
    void WriteLine(std::string_view text) override;
};