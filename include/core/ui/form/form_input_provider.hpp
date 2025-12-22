#pragma once
#include <optional>
#include <string>
#include <vector>

namespace form
{

    class FormInputProvider
    {
    public:
        virtual ~FormInputProvider() = default;

        /// Displays cancellation instructions once at the start of form input
        virtual void DisplayCancellationInstructions() = 0;

        /// Reads text input. Returns nullopt if user cancels.
        virtual std::optional<std::string> ReadText(const std::string &prompt) = 0;

        /// Reads selection input. Returns nullopt if user cancels.
        virtual std::optional<std::string> ReadSelection(const std::string &title,
                                                         const std::vector<std::string> &options) = 0;
    };

} // namespace form
