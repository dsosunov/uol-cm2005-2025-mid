#pragma once
#include <string>
class Input {
 public:
  virtual ~Input() = default;
  virtual std::string ReadLine() = 0;
};