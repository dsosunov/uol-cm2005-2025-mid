#pragma once

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "core/ui/form/form.hpp"
#include "forms/candlestick/candlestick_form_data_provider.hpp"

namespace candlestick
{

    class CandlestickForm : public form::Form
    {
    public:
        CandlestickForm(std::shared_ptr<form::FormInputProvider> input_provider,
                        std::shared_ptr<Output> output,
                        std::shared_ptr<CandlestickFormDataProvider> data_provider);

    private:
        static std::vector<std::shared_ptr<form::Field>> SetupFormLayout(
            std::shared_ptr<CandlestickFormDataProvider> data_provider);
    };

} // namespace candlestick
