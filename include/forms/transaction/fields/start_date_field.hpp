#pragma once
#include "core/ui/form/data_source.hpp"
#include "dto/activity_summary.hpp"
#include "forms/shared/fields/date_fields.hpp"

#include <memory>

namespace transaction_forms
{
class StartDateField : public forms::shared::StartDateField<dto::ActivitySummary>
{
  public:
    explicit StartDateField(std::shared_ptr<form::DataSource> data_source);
};
} // namespace transaction_forms