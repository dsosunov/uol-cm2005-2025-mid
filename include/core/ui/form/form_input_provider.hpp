#pragma once
#include <optional>
#include <string>
#include <vector>

namespace form {

/// Abstraction for providing input to forms.
/// This decouples forms from specific input mechanisms (menu system, CLI, web, etc.)
class FormInputProvider {
public:
    virtual ~FormInputProvider() = default;

    /// Read a line of text input with a prompt
    /// @param prompt The prompt to display to the user
    /// @return The text entered by the user
    virtual std::string ReadText(const std::string& prompt) = 0;

    /// Present a selection menu and get user's choice
    /// @param title The title/prompt for the selection
    /// @param options The list of options to choose from
    /// @return The selected option, or std::nullopt if cancelled
    virtual std::optional<std::string> ReadSelection(
        const std::string& title,
        const std::vector<std::string>& options) = 0;
};

} // namespace form
