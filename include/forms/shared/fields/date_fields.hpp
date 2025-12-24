#pragma once
#include "core/ui/form/data_source.hpp"
#include "core/ui/form/fields/selection_field.hpp"
#include "core/utils/time_utils.hpp"

#include <memory>
#include <optional>

namespace forms::shared
{
template <typename TDTO> class StartDateField : public form::SelectionField
{
  public:
    using MemberPtr = std::optional<utils::TimePoint> TDTO::*;
    explicit StartDateField(std::shared_ptr<form::DataSource> data_source, MemberPtr member_ptr);
};
template <typename TDTO> class EndDateField : public form::SelectionField
{
  public:
    using MemberPtr = std::optional<utils::TimePoint> TDTO::*;
    explicit EndDateField(std::shared_ptr<form::DataSource> data_source, MemberPtr member_ptr);
};
} // namespace forms::shared