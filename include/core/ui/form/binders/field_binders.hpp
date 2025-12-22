#pragma once
#include <any>
#include <functional>
#include <string>
#include "core/ui/form/form_context.hpp"

namespace form
{
    template <typename T, typename FieldType>
    class SimpleFieldBinder
    {
    public:
        using MemberPtr = FieldType T::*;

        explicit SimpleFieldBinder(MemberPtr member_ptr)
            : member_ptr_(member_ptr) {}

        void operator()(std::any &target, const std::string &value,
                        const FormContext &context) const
        {
            auto &obj = std::any_cast<std::reference_wrapper<T>>(target).get();
            obj.*member_ptr_ = value;
        }

    private:
        MemberPtr member_ptr_;
    };

} // namespace form
