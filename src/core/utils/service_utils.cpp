#include "core/utils/service_utils.hpp"
namespace services
{
    int ServiceUtils::GetEffectiveUserId(std::optional<int> user_id, int default_user_id)
    {
        return user_id.value_or(default_user_id);
    }
}