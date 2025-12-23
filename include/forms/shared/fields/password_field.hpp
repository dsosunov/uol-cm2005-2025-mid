#pragma once
#include "core/ui/form/fields/text_field.hpp"
namespace forms::shared
{
    template <typename TDTO>
    class PasswordField : public form::TextField
    {
    public:
        using MemberPtr = std::string TDTO::*;
        explicit PasswordField(const std::string &field_name, const std::string &prompt, MemberPtr member_ptr);
    };
}