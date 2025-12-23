#pragma once
#include <string>
#include <string_view>
namespace utils
{
    class OutputFormatter
    {
    public:
        static std::string SectionHeader(std::string_view title);
        static std::string SuccessHeader(std::string_view operation);
        static std::string FailureHeader(std::string_view operation);
        static std::string CancellationMessage(std::string_view operation);
    };
}