#pragma once
#include <memory>
#include "core/ui/form/data_source.hpp"
#include "forms/shared/fields/date_fields.hpp"
#include "dto/activity_summary.hpp"
namespace transaction_forms
{
  class EndDateField : public forms::shared::EndDateField<dto::ActivitySummary>
  {
  public:
    explicit EndDateField(std::shared_ptr<form::DataSource> data_source);
  };
}