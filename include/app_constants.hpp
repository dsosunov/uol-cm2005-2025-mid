#pragma once

#include <map>
#include <set>
#include <string>
#include <string_view>

namespace app
{

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

inline double SyntheticPrice(std::string_view base, std::string_view quote)
{
    auto it_base = kSyntheticPriceInUSDT.find(std::string(base));
    auto it_quote = kSyntheticPriceInUSDT.find(std::string(quote));
    if (it_base == kSyntheticPriceInUSDT.end() || it_quote == kSyntheticPriceInUSDT.end() ||
        it_quote->second <= 0.0)
    {
        return 0.0;
    }

    return it_base->second / it_quote->second;
}

} // namespace app
