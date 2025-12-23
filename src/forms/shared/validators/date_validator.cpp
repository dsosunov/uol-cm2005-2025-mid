#include "forms/shared/validators/date_validator.hpp"

#include <cctype>

namespace forms::shared
{

  form::ValidationResult DateValidator::ValidateNumericString(const std::string &str, const std::string &field_name) const
  {
    for (char c : str)
    {
      if (!std::isdigit(c))
      {
        return form::ValidationResult::Invalid(field_name + " must be numeric");
      }
    }
    return form::ValidationResult::Valid();
  }

  form::ValidationResult DateValidator::ValidateDay(const std::string &day_str) const
  {
    if (day_str.empty() || day_str.length() > 2)
    {
      return form::ValidationResult::Invalid("Day must be 1-2 digits");
    }

    auto numeric_result = ValidateNumericString(day_str, "Day");
    if (!numeric_result.is_valid)
    {
      return numeric_result;
    }

    int day = std::stoi(day_str);
    if (day < 1 || day > 31)
    {
      return form::ValidationResult::Invalid("Day must be between 1 and 31");
    }

    return form::ValidationResult::Valid();
  }

  form::ValidationResult DateValidator::ValidateMonth(const std::string &month_str) const
  {
    if (month_str.empty() || month_str.length() > 2)
    {
      return form::ValidationResult::Invalid("Month must be 1-2 digits");
    }

    auto numeric_result = ValidateNumericString(month_str, "Month");
    if (!numeric_result.is_valid)
    {
      return numeric_result;
    }

    int month = std::stoi(month_str);
    if (month < 1 || month > 12)
    {
      return form::ValidationResult::Invalid("Month must be between 1 and 12");
    }

    return form::ValidationResult::Valid();
  }

  form::ValidationResult DateValidator::ValidateYear(const std::string &year_str) const
  {
    if (year_str.length() != 4)
    {
      return form::ValidationResult::Invalid("Year must be 4 digits");
    }

    auto numeric_result = ValidateNumericString(year_str, "Year");
    if (!numeric_result.is_valid)
    {
      return numeric_result;
    }

    int year = std::stoi(year_str);
    if (year < 1900 || year > 2100)
    {
      return form::ValidationResult::Invalid("Year must be between 1900 and 2100");
    }

    return form::ValidationResult::Valid();
  }

  form::ValidationResult DateValidator::ValidateDMYFormat(const std::string &value) const
  {
    size_t first_slash = value.find('/');
    size_t second_slash = value.find('/', first_slash + 1);

    if (second_slash == std::string::npos)
    {
      return form::ValidationResult::Invalid("Invalid date format. Expected DD/MM/YYYY");
    }

    std::string day_str = value.substr(0, first_slash);
    std::string month_str = value.substr(first_slash + 1, second_slash - first_slash - 1);
    std::string year_str = value.substr(second_slash + 1);

    auto day_result = ValidateDay(day_str);
    if (!day_result.is_valid)
    {
      return day_result;
    }

    auto month_result = ValidateMonth(month_str);
    if (!month_result.is_valid)
    {
      return month_result;
    }

    auto year_result = ValidateYear(year_str);
    if (!year_result.is_valid)
    {
      return year_result;
    }

    return form::ValidationResult::Valid();
  }

  form::ValidationResult DateValidator::ValidateYMDFormat(const std::string &value) const
  {
    size_t first_dash = value.find('-');
    size_t second_dash = value.find('-', first_dash + 1);

    if (second_dash == std::string::npos)
    {
      return form::ValidationResult::Invalid("Invalid date format. Expected YYYY-MM-DD");
    }

    std::string year_str = value.substr(0, first_dash);
    std::string month_str = value.substr(first_dash + 1, second_dash - first_dash - 1);
    std::string day_str = value.substr(second_dash + 1);

    auto year_result = ValidateYear(year_str);
    if (!year_result.is_valid)
    {
      return year_result;
    }

    auto month_result = ValidateMonth(month_str);
    if (!month_result.is_valid)
    {
      return month_result;
    }

    auto day_result = ValidateDay(day_str);
    if (!day_result.is_valid)
    {
      return day_result;
    }

    return form::ValidationResult::Valid();
  }

  form::ValidationResult DateValidator::Validate(const std::string &value,
                                                 const form::FormContext &context) const
  {
    if (value.empty())
    {
      return form::ValidationResult::Invalid("Date cannot be empty");
    }

    size_t first_slash = value.find('/');
    if (first_slash != std::string::npos)
    {
      return ValidateDMYFormat(value);
    }

    size_t first_dash = value.find('-');
    if (first_dash != std::string::npos)
    {
      return ValidateYMDFormat(value);
    }

    return form::ValidationResult::Invalid("Invalid date format. Expected DD/MM/YYYY or YYYY-MM-DD");
  }

  std::optional<std::string> DateValidator::GetHint() const
  {
    return "Enter date in DD/MM/YYYY format";
  }

} // namespace forms::shared
