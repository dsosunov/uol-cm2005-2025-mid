#include "core/ui/menu/menu_input.hpp"
#include "core/ui/menu/menu_renderer.hpp"
#include "services/authentication_context.hpp"
#include "services/authentication_service.hpp"

#include <any>
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace
{
class TestOutput final : public Output
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

    bool ContainsSubstring(const std::string& needle) const
    {
        for (const auto& l : lines_)
        {
            if (l.contains(needle))
            {
                return true;
            }
        }

        for (const auto& c : chunks_)
        {
            if (c.contains(needle))
            {
                return true;
            }
        }

        return false;
    }

  private:
    std::vector<std::string> lines_;
    std::vector<std::string> chunks_;
};

class TestInput final : public Input
{
  public:
    explicit TestInput(std::vector<std::string> scripted) : scripted_(std::move(scripted))
    {
    }

    std::string ReadLine() override
    {
        if (index_ >= scripted_.size())
        {
            return "";
        }
        std::string line = scripted_[index_];
        ++index_;
        return line;
    }

  private:
    std::vector<std::string> scripted_;
    size_t index_ = 0;
};
} // namespace

TEST(MenuAuthVisibility, RendererHidesProtectedItemsWhenGuest)
{
    auto auth_context = std::make_shared<services::AuthenticationContext>();
    auto auth_service = std::make_shared<services::AuthenticationService>(auth_context);
    auto output = std::make_shared<TestOutput>();

    MenuRenderer renderer(output, auth_service);

    MenuNode root("Root");
    root.AddChild(std::make_unique<MenuNode>("Private", nullptr, std::any{}, true));
    root.AddChild(std::make_unique<MenuNode>("Public", nullptr, std::any{}, false));

    renderer.RenderMenu(root);

    EXPECT_FALSE(output->ContainsSubstring("Private"));
    EXPECT_TRUE(output->ContainsSubstring("1) Public"));
    EXPECT_TRUE(output->ContainsSubstring("Choose an option (0-1)"));
}

TEST(MenuAuthVisibility, RendererShowsProtectedItemsWhenAuthenticated)
{
    auto auth_context = std::make_shared<services::AuthenticationContext>();
    auto auth_service = std::make_shared<services::AuthenticationService>(auth_context);
    auth_service->SetCurrentUser(
        services::AuthenticatedUser{1, "user1", "User One", "u@example.com"});

    auto output = std::make_shared<TestOutput>();
    MenuRenderer renderer(output, auth_service);

    MenuNode root("Root");
    root.AddChild(std::make_unique<MenuNode>("Private", nullptr, std::any{}, true));
    root.AddChild(std::make_unique<MenuNode>("Public", nullptr, std::any{}, false));

    renderer.RenderMenu(root);

    EXPECT_TRUE(output->ContainsSubstring("Private"));
    EXPECT_TRUE(output->ContainsSubstring("Public"));
    EXPECT_TRUE(output->ContainsSubstring("Choose an option (0-2)"));
}

TEST(MenuAuthVisibility, InputSelectionMapsToVisibleItems)
{
    auto auth_context = std::make_shared<services::AuthenticationContext>();
    auto auth_service = std::make_shared<services::AuthenticationService>(auth_context);

    auto output = std::make_shared<TestOutput>();
    auto renderer = std::make_shared<MenuRenderer>(output, auth_service);
    auto input = std::make_shared<TestInput>(std::vector<std::string>{"1"});
    MenuInput menu_input(input, renderer, auth_service);

    MenuNode root("Root");
    root.AddChild(std::make_unique<MenuNode>("Private", nullptr, std::any{}, true));
    root.AddChild(std::make_unique<MenuNode>("Public", nullptr, std::any{}, false));

    MenuNode* selected = menu_input.ReadSelection(root);
    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->Title(), "Public");
}
