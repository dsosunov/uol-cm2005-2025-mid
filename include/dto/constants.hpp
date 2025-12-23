#pragma once
#include <string>
#include <vector>

namespace dto
{

  namespace timeframe
  {
    inline constexpr const char *DAILY = "daily";
    inline constexpr const char *MONTHLY = "monthly";
    inline constexpr const char *YEARLY = "yearly";

    inline std::vector<std::string> GetAll() { return {DAILY, MONTHLY, YEARLY}; }
  } // namespace timeframe

  namespace order_type
  {
    inline constexpr const char *ASKS = "asks";
    inline constexpr const char *BIDS = "bids";

    inline std::vector<std::string> GetAll() { return {ASKS, BIDS}; }
  } // namespace order_type

} // namespace dto
