#include "core/utils/output_formatter.hpp"

#include <format>

namespace utils
{
std::string OutputFormatter::SectionHeader(std::string_view title)
{
    return std::format("=== {} ===", title);
}

std::string OutputFormatter::SuccessHeader(std::string_view operation)
{
    return std::format("=== {} Successful ===", operation);
}

std::string OutputFormatter::FailureHeader(std::string_view operation)
{
    return std::format("=== {} Failed ===", operation);
}

std::string OutputFormatter::CancellationMessage(std::string_view operation)
{
    return std::format("{} cancelled by user.", operation);
}
} // namespace utils