#pragma once

#include <map>
#include <set>
#include <string>
#include <string_view>

namespace app
{

// Password rules (used by UI validators and services).
inline constexpr std::size_t kPasswordMinLength = 6;
inline constexpr std::size_t kPasswordMaxLength = 50;

// User profile rules (used by UI validators).
inline constexpr std::size_t kUsernameLength = 10;
inline constexpr std::size_t kFullNameMinLength = 2;
inline constexpr std::size_t kFullNameMaxLength = 100;

// Wallet amount rules (used by UI validators).
inline constexpr std::size_t kWalletAmountMaxDecimalPlaces = 2;
inline constexpr double kWalletAmountMinExclusive = 0.0;
inline constexpr double kWalletAmountMax = 1000000000.0;

// Wallet amount hint text currently communicates a different precision than the validator enforces.
// Keep it centralized so it can be changed intentionally.
inline constexpr std::size_t kWalletAmountHintMaxDecimalPlaces = 8;

// Trading/wallet currencies supported by the application.
inline const std::set<std::string, std::less<>> kSupportedCurrencies = {"BTC", "DOGE", "ETH",
                                                                        "USDT"};

// Initial wallet balances granted on user registration.
inline const std::map<std::string, double, std::less<>> kInitialBalances = {
    {"BTC", 1.0},
    {"DOGE", 1000.0},
    {"ETH", 10.0},
    {"USDT", 10000.0},
};

// Deterministic synthetic prices used for simulation when the system has no market data yet.
// Prices are quoted in USDT.
inline const std::map<std::string, double, std::less<>> kSyntheticPriceInUSDT = {
    {"BTC", 10.0},
    {"ETH", 5.0},
    {"DOGE", 1.0},
    {"USDT", 1.0},
};

// Deterministic exchange rates to USD for computing wallet totals.
// Since kSyntheticPriceInUSDT is quoted in USDT, we also define the USDT->USD peg.
inline constexpr double kUSDTToUSD = 1.0;

// Default exchange rate to use when a currency is missing from kExchangeRateToUSD.
// Requirement: use 1 as the default.
inline constexpr double kDefaultExchangeRateToUSD = 1.0;

// Exchange rate table: 1 unit of currency * rate = USD.
inline const std::map<std::string, double, std::less<>> kExchangeRateToUSD = {
    {"USD", 1.0},
    {"USDT", kUSDTToUSD},
    {"BTC", kSyntheticPriceInUSDT.at("BTC") * kUSDTToUSD},
    {"ETH", kSyntheticPriceInUSDT.at("ETH") * kUSDTToUSD},
    {"DOGE", kSyntheticPriceInUSDT.at("DOGE") * kUSDTToUSD},
};

} // namespace app
