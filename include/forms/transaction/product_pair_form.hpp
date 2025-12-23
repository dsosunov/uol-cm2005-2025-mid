#pragma once

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "core/ui/form/form.hpp"
#include "core/ui/form/form_input_provider.hpp"
#include "core/ui/io/output.hpp"

namespace transaction_forms
{

    class ProductPairForm : public form::Form
    {
    public:
        ProductPairForm(std::shared_ptr<form::FormInputProvider> input_provider,
                        std::shared_ptr<Output> output, const std::set<std::string, std::less<>> &allowed_currencies);

    private:
        static std::vector<std::shared_ptr<form::Field>> SetupFormLayout(
            const std::set<std::string, std::less<>> &allowed_currencies);
    };

} // namespace transaction_forms
