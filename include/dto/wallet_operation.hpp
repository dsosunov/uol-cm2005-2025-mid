#pragma once
#include <string>
namespace dto
{
  struct WalletOperation
  {
    std::string currency;
    std::string amount;
  };
}