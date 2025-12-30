#include "core/ui/form/fields/text_field.hpp"
#include "core/ui/form/form.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/form/validator.hpp"

#include <any>
#include <gtest/gtest.h>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace
{
class ScriptedFormInputProvider final : public form::FormInputProvider
{
  public:
    explicit ScriptedFormInputProvider(std::vector<std::optional<std::any>> scripted)
        : scripted_(std::move(scripted))
    {
    }

    std::optional<std::any> ReadField(const form::Field&, const form::FormContext&) override
    {
        if (index_ >= scripted_.size())
        {
            // Default to empty string if test forgets to provide enough input.
            return std::any{std::string{}};
        }

        auto v = scripted_[index_];
        ++index_;
        if (!v.has_value())
        {
            return std::nullopt;
        }

        return *v;
    }

  private:
    std::vector<std::optional<std::any>> scripted_;
    size_t index_ = 0;
};

class CollectingOutput final : public Output
{
  public:
    void Write(const std::string& text) override
    {
        chunks_.push_back(text);
    }

    void WriteLine(const std::string& text) override
    {
        lines_.push_back(text);
    }

    bool ContainsLineSubstring(const std::string& needle) const
    {
        for (const auto& l : lines_)
        {
            if (l.contains(needle))
            {
                return true;
            }
        }
        return false;
    }

    size_t CountPromptWrites(const std::string& prompt_prefix) const
    {
        size_t count = 0;
        for (const auto& c : chunks_)
        {
            if (c.rfind(prompt_prefix, 0) == 0)
            {
                ++count;
            }
        }
        return count;
    }

  private:
    std::vector<std::string> chunks_;
    std::vector<std::string> lines_;
};

class RejectOnceValidator final : public form::Validator
{
  public:
    explicit RejectOnceValidator(std::string bad_value) : bad_value_(std::move(bad_value))
    {
    }

    form::ValidationResult Validate(const std::string& value,
                                    const form::FormContext&) const override
    {
        if (!rejected_ && value == bad_value_)
        {
            rejected_ = true;
            return form::ValidationResult::Invalid("bad input");
        }

        return form::ValidationResult::Valid();
    }

  private:
    std::string bad_value_;
    mutable bool rejected_ = false;
};

struct Target
{
    std::string name;
};
} // namespace

TEST(FormRead, RetriesTextFieldUntilValidOrCancelled)
{
    Target t;

    auto validator = std::make_shared<RejectOnceValidator>("bad");

    auto field = std::make_shared<form::TextField>(
        "name", "Name",
        form::ValueBinder{[](std::any& target, const std::any& value, const form::FormContext&) {
            auto& out = std::any_cast<std::reference_wrapper<Target>>(target).get();
            out.name = std::any_cast<const std::string&>(value);
        }},
        validator);

    auto output = std::make_shared<CollectingOutput>();
    auto input_provider =
        std::make_shared<ScriptedFormInputProvider>(std::vector<std::optional<std::any>>{
            std::any{std::string{"bad"}}, std::any{std::string{"good"}}});

    form::Form f({field}, input_provider, output);

    const auto result = f.Read(t);

    EXPECT_EQ(result, form::FormReadResult::kSuccess);
    EXPECT_EQ(t.name, "good");

    EXPECT_TRUE(output->ContainsLineSubstring("Error: bad input"));
    EXPECT_GE(output->CountPromptWrites("Name"), 2u);
}
