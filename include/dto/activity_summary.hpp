#pragma once
#include <string>

namespace dto
{

    struct ActivitySummary
    {
        std::string timeframe; // Use dto::timeframe constants
        std::string start_date;
        std::string end_date;
    };

} // namespace dto
