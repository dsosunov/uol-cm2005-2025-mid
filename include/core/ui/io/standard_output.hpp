#pragma once
#include <string>

#include "core/ui/io/output.hpp"

class StandardOutput : public Output {
 public:
  StandardOutput();
  void Write(const std::string& text) override;
  void WriteLine(const std::string& text) override;
};
