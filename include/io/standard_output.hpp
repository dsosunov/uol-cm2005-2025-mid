#pragma once
#include "io/output.hpp"
#include <string>

class StandardOutput : public Output
{
public:
    void Write(const std::string &text) override;
    void WriteLine(const std::string &text) override;
};
