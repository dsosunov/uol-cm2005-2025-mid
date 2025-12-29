#pragma once
#include "core/ui/io/output.hpp"
#include "core/ui/menu/menu_node.hpp"

#include <memory>
#include <optional>

namespace services
{
class AuthenticationService;
}

class MenuRenderer
{
  public:
    MenuRenderer(std::shared_ptr<Output> output,
                 std::shared_ptr<services::AuthenticationService> auth_service);
    void RenderMenu(const MenuNode& current,
                    std::optional<size_t> default_option = std::nullopt) const;
    void RenderMenu(const MenuNode& current, std::string_view status_line,
                    std::optional<size_t> default_option = std::nullopt) const;
    void RenderActionHeader(const MenuNode& node) const;
    void RenderInvalidChoice() const;

  private:
    std::shared_ptr<Output> output_;
    std::shared_ptr<services::AuthenticationService> auth_service_;
    std::string BuildBreadcrumbPath(const MenuNode& current) const;
};