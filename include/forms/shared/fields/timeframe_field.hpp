#pragma once
#include "core/ui/form/fields/selection_field.hpp"
#include "dto/constants.hpp"
namespace forms::shared
{
    template <typename TDTO>
    class TimeframeField : public form::SelectionField
    {
    public:
        using MemberPtr = dto::Timeframe TDTO::*;
        explicit TimeframeField(MemberPtr member_ptr);
    };
}