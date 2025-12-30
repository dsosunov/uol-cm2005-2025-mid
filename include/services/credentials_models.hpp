#pragma once

#include <cstdint>

namespace services
{

struct CredentialRecord
{
    int user_id;
    std::uint64_t password_hash;
};

} // namespace services
