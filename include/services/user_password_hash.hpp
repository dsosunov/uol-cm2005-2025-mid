#pragma once

#include <cstdint>
#include <string_view>

namespace services::internal
{

// NOTE: I intentionally do NOT use std::hash here.
// - The C++ standard does not require std::hash to be stable across compilers, standard
//   library implementations, builds, or platforms.
// - That makes it a poor choice for values that are persisted to disk (CSV) and must compare
//   consistently when the program is rebuilt.
// This is a deterministic, compiler-independent hash (FNV-1a 64-bit).
// Security note: this is NOT a secure password hashing algorithm.
inline std::uint64_t HashPassword(std::string_view password)
{
    std::uint64_t hash = 14695981039346656037ULL; // FNV offset basis
    for (unsigned char ch : password)
    {
        hash ^= static_cast<std::uint64_t>(ch);
        hash *= 1099511628211ULL; // FNV prime
    }
    return hash;
}

} // namespace services::internal
