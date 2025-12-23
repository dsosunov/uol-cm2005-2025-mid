#include "core/ui/menu/menu_renderer.hpp"
#include "core/utils/output_formatter.hpp"
#include <format>
#include <string>
MenuRenderer::MenuRenderer(std::shared_ptr<Output> output) : output_(std::move(output)) {}
void MenuRenderer::RenderMenu(const MenuNode &current) const
{
  if (!current.IsRoot())
  {
    RenderBreadcrumbs(current);
  }
  output_->WriteLine(utils::OutputFormatter::SectionHeader(current.Title()));
  const auto &items = current.Children();
  for (size_t i = 0; i < items.size(); ++i)
  {
    output_->WriteLine(std::format("{}) {}", i + 1, items[i]->Title()));
  }
  output_->WriteLine(std::format("0) {}", current.IsRoot() ? "Exit" : "Back"));
  output_->Write(std::format("Select option (0-{}): ", items.size()));
}
void MenuRenderer::RenderActionHeader(const MenuNode &node) const
{
  output_->WriteLine(utils::OutputFormatter::SectionHeader(node.Title()));
}
void MenuRenderer::RenderInvalidChoice() const { output_->WriteLine("Invalid choice."); }
void MenuRenderer::RenderBreadcrumbs(const MenuNode &current) const
{
  std::vector<const MenuNode *> path;
  const MenuNode *node = &current;
  while (node)
  {
    path.push_back(node);
    node = node->Parent();
  }
  std::string breadcrumb;
  for (auto it = path.rbegin(); it != path.rend(); ++it)
  {
    if (it != path.rbegin())
    {
      breadcrumb += " > ";
    }
    breadcrumb += (*it)->Title();
  }
  output_->WriteLine(breadcrumb);
}