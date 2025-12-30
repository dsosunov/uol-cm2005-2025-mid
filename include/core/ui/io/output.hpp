#pragma once
#include <string_view>
class Output
{
  public:
    virtual ~Output() = default;
    virtual void Write(std::string_view text) = 0;
    virtual void WriteLine(std::string_view text) = 0;
};