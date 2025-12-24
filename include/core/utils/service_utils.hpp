#pragma once
#include <optional>
namespace services
{
class ServiceUtils
{
  public:
    static int GetEffectiveUserId(std::optional<int> user_id, int default_user_id);
};
} // namespace services