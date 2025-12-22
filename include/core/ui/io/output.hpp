#pragma once
#include <string>

class Output {
 public:
  virtual ~Output() = default;
  virtual void Write(const std::string& text) = 0;
  virtual void WriteLine(const std::string& text) = 0;
};
