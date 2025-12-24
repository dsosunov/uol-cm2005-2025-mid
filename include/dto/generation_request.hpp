#pragma once
#include <string>

namespace dto
{
struct GenerationRequest
{
    std::string confirmation; // "yes" or empty string for cancel
};
} // namespace dto
