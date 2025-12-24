#pragma once
#include "core/ui/io/output.hpp"

#include <string>

class StandardOutput : public Output
{
  public:
    StandardOutput();
    void Write(const std::string& text) override;
    void WriteLine(const std::string& text) override;
};