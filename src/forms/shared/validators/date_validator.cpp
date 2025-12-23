#include "forms/shared/validators/date_validator.hpp"

#include <cctype>
#include <string>
#include <string_view>

namespace forms::shared
{

  form::ValidationResult DateValidator::ValidateNumericString(std::string_view str, std::string_view field_name) const
  {
    for (char c : str)
    {
      if (!std::isdigit(c))
      {
        return form::ValidationResult::Invalid(std::string(field_name) + " must be numeric");
      }
    }
    return form::ValidationResult::Valid();
  }

  form::ValidationResult DateValidator::ValidateDay(std::string_view day_str) const
  {
    if (day_str.empty() || day_str.length() > 2)
    {
      return form::ValidationResult::Invalid("Day must be 1-2 digits");
    }

    if (auto numeric_result = ValidateNumericString(day_str, "Day"); !numeric_result.is_valid)
    {
      return numeric_result;
    }

    int day = std::stoi(std::string(day_str));
    if (day < 1 || day > 31)
    {
      return form::ValidationResult::Invalid("Day must be between 1 and 31");
    }

    return form::ValidationResult::Valid();
  }

  form::ValidationResult DateValidator::ValidateMonth(std::string_view month_str) const
  {
    if (month_str.empty() || month_str.length() > 2)
    {
      return form::ValidationResult::Invalid("Month must be 1-2 digits");
    }

    if (auto numeric_result = ValidateNumericString(month_str, "Month"); !numeric_result.is_valid)
    {
      return numeric_result;
    }

    int month = std::stoi(std::string(month_str));
    if (month < 1 || month > 12)
    {
      return form::ValidationResult::Invalid("Month must be between 1 and 12");
    }

    return form::ValidationResult::Valid();
  }

  form::ValidationResult DateValidator::ValidateYear(std::string_view year_str) const
  {
    if (year_str.length() != 4)
    {
      return form::ValidationResult::Invalid("Year must be 4 digits");
    }

    if (auto numeric_result = ValidateNumericString(year_str, "Year"); !numeric_result.is_valid)
    {
      return numeric_result;
    }

    int year = std::stoi(std::string(year_str));
    if (year < 1900 || year > 2100)
    {
      return form::ValidationResult::Invalid("Year must be between 1900 and 2100");
    }

    return form::ValidationResult::Valid();
  }

  form::ValidationResult DateValidator::ValidateDMYFormat(std::string_view value) const
  {
    size_t first_slash = value.find('/');
    size_t second_slash = value.find('/', first_slash + 1);

    if (second_slash == std::string::npos)
    {
      return form::ValidationResult::Invalid("Invalid date format. Expected DD/MM/YYYY");
    }

    std::string_view day_str = value.substr(0, first_slash);
    std::string_view month_str = value.substr(first_slash + 1, second_slash - first_slash - 1);
    std::string_view year_str = value.substr(second_slash + 1);

    if (auto day_result = ValidateDay(day_str); !day_result.is_valid)
    {
      return day_result;
    }

    if (auto month_result = ValidateMonth(month_str); !month_result.is_valid)
    {
      return month_result;
    }

    if (auto year_result = ValidateYear(year_str); !year_result.is_valid)
    {
      return year_result;
    }

    return form::ValidationResult::Valid();
  }

  form::ValidationResult DateValidator::ValidateYMDFormat(std::string_view value) const
  {
    size_t first_dash = value.find('-');
    size_t second_dash = value.find('-', first_dash + 1);

    if (second_dash == std::string::npos)
    {
      return form::ValidationResult::Invalid("Invalid date format. Expected YYYY-MM-DD");
    }

    std::string_view year_str = value.substr(0, first_dash);
    std::string_view month_str = value.substr(first_dash + 1, second_dash - first_dash - 1);
    std::string_view day_str = value.substr(second_dash + 1);

    if (auto year_result = ValidateYear(year_str); !year_result.is_valid)
    {
      return year_result;
    }

    if (auto month_result = ValidateMonth(month_str); !month_result.is_valid)
    {
      return month_result;
    }

    if (auto day_result = ValidateDay(day_str); !day_result.is_valid)
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

    if (size_t first_slash = value.find('/'); first_slash != std::string::npos)
    {
      return ValidateDMYFormat(std::string_view(value));
    }

    if (size_t first_dash = value.find('-'); first_dash != std::string::npos)
    {
      return ValidateYMDFormat(std::string_view(value));
    }

    return form::ValidationResult::Invalid("Invalid date format. Expected DD/MM/YYYY or YYYY-MM-DD");
  }

  std::optional<std::string> DateValidator::GetHint() const
  {
    return "Enter date in DD/MM/YYYY format";
  }

} // namespace forms::shared
