#pragma once
#include <string>
#include <vector>

namespace dto {

namespace timeframe {
inline constexpr const char* DAILY = "daily";
inline constexpr const char* MONTHLY = "monthly";
inline constexpr const char* YEARLY = "yearly";

inline std::vector<std::string> GetAll() { return {DAILY, MONTHLY, YEARLY}; }
}  // namespace timeframe

namespace order_type {
inline constexpr const char* ASKS = "asks";
inline constexpr const char* BIDS = "bids";

inline std::vector<std::string> GetAll() { return {ASKS, BIDS}; }
}  // namespace order_type

namespace sample_dates {
inline std::vector<std::string> GetDailySamples() {
  return {"2025-12-01", "2025-12-15", "2025-12-22"};
}

inline std::vector<std::string> GetMonthlySamples() {
  return {"2025-01", "2025-02", "2025-03", "2025-04", "2025-05", "2025-06",
          "2025-07", "2025-08", "2025-09", "2025-10", "2025-11", "2025-12"};
}

inline std::vector<std::string> GetYearlySamples() {
  return {"2020", "2021", "2022", "2023", "2024", "2025"};
}
}  // namespace sample_dates

}  // namespace dto
