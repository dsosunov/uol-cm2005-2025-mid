#pragma once
#include "core/actions/action_context.hpp"
#include "core/ui/form/form.hpp"
#include "core/utils/output_formatter.hpp"

#include <format>
#include <string>
#include <string_view>

struct MenuAction
{
    virtual ~MenuAction() = default;
    virtual void Execute(ActionContext& context) = 0;

  protected:
    virtual const char* GetOperationName() const
    {
        return "Operation";
    }

    bool HandleFormCancellation(form::FormReadResult result, std::string_view operation_name,
                                ActionContext& context) const
    {
        if (result == form::FormReadResult::kCancelled)
        {
            context.output->WriteLine("");
            context.output->WriteLine(utils::OutputFormatter::CancellationMessage(operation_name));
            return true;
        }
        return false;
    }

    void DisplaySuccessHeader(ActionContext& context) const
    {
        context.output->WriteLine(utils::OutputFormatter::SuccessHeader(GetOperationName()));
    }

    void DisplayFailureHeader(std::string_view message, ActionContext& context) const
    {
        context.output->WriteLine(utils::OutputFormatter::FailureHeader(GetOperationName()));
        if (!message.empty())
        {
            context.output->WriteLine(std::format("Error: {}", message));
        }
    }

    template <typename T>
    void DisplayField(std::string_view label, const T& value, ActionContext& context) const
    {
        context.output->WriteLine(std::format("{}: {}", label, value));
    }

    void WriteLine(std::string_view text, ActionContext& context) const
    {
        context.output->WriteLine(std::string(text));
    }

    void WriteEmptyLine(ActionContext& context) const
    {
        context.output->WriteLine("");
    }

    void DisplayResultFooter(ActionContext& context) const
    {
        context.output->WriteLine("===========================");
        context.output->WriteLine("");
    }
};