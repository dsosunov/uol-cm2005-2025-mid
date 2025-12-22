#include <memory>
#include <string>
#include <vector>

#include "io.hpp"
#include "actions.hpp"
#include "menu_node.hpp"
#include "menu_renderer.hpp"

// ----------------------------
// 4) Input (handles reading, validation, and selection)
// ----------------------------
class MenuInput
{
public:
    MenuInput(IInput& input, MenuRenderer& renderer) : _input(input), _renderer(renderer) {}

    // Returns the next node to navigate to, or nullptr for exit
    // Keeps prompting until valid input is received
    MenuNode* ReadSelection(const MenuNode& current) const
    {
        while (true)
        {
            std::string line = _input.ReadLine();

            int option;
            try
            {
                option = std::stoi(line);
            }
            catch (...)
            {
                _renderer.RenderInvalidChoice();
                _renderer.RenderMenu(current);
                continue;
            }

            const size_t childCount = current.Children().size();

            // Handle 0: Exit (from root) or Back (from non-root)
            if (option == 0)
            {
                return current.IsRoot() ? nullptr : current.Parent();
            }

            // Validate range for child selection
            const int index = option - 1;
            if (index < 0 || static_cast<size_t>(index) >= childCount)
            {
                _renderer.RenderInvalidChoice();
                _renderer.RenderMenu(current);
                continue;
            }

            return current.Children()[static_cast<size_t>(index)].get();
        }
    }

private:
    IInput& _input;
    MenuRenderer& _renderer;
};

// ----------------------------
// 5) Engine (loop + navigation)
// ----------------------------
class MenuEngine
{
public:
    // Constructor: receives menu items (root), renderer, and input handler
    MenuEngine(std::unique_ptr<MenuNode> root, MenuRenderer renderer, MenuInput input)
        : _root(std::move(root)), _renderer(std::move(renderer)), _input(std::move(input))
    {
        _stack.push_back(_root.get());
    }

    void Run()
    {
        while (true)
        {
            auto* current = _stack.back();

            _renderer.RenderMenu(*current);
            MenuNode* selected = _input.ReadSelection(*current);

            // Exit if nullptr returned
            if (!selected)
            {
                return;
            }

            // Navigate to child if it has children
            if (selected->HasChildren())
            {
                _stack.push_back(selected);
            }
            // Execute action if it's a leaf node
            else if (selected->HasAction())
            {
                selected->ExecuteAction();
            }
            // Otherwise go back (selected is parent)
            else
            {
                _stack.pop_back();
            }
        }
    }

private:
    std::unique_ptr<MenuNode> _root;
    std::vector<MenuNode*> _stack;

    MenuRenderer _renderer;
    MenuInput _input;
};

// ----------------------------
// 6) Menu Builder (fluent API)
// ----------------------------
class MenuBuilder
{
public:
    explicit MenuBuilder(std::string title)
        : _root(std::make_unique<MenuNode>(title)), _current(_root.get())
    {
    }

    MenuBuilder& AddLeaf(const std::string& title, std::unique_ptr<IMenuAction> action)
    {
        _current->AddChild(std::make_unique<MenuNode>(title, std::move(action)));
        return *this;
    }

    MenuBuilder& AddBranch(const std::string& title)
    {
        auto branch = std::make_unique<MenuNode>(title);
        MenuNode* branchPtr = branch.get();
        _current->AddChild(std::move(branch));
        _current = branchPtr;
        return *this;
    }

    MenuBuilder& Parent()
    {
        if (_current->Parent())
        {
            _current = _current->Parent();
        }
        return *this;
    }

    std::unique_ptr<MenuNode> Build()
    {
        return std::move(_root);
    }

private:
    std::unique_ptr<MenuNode> _root;
    MenuNode* _current;
};

// ----------------------------
// 7) Build your menu tree here
// ----------------------------
std::unique_ptr<MenuNode> BuildMenu()
{
    return MenuBuilder("Main Menu")
        .AddBranch("Tools")
            .AddLeaf("Say Hello", std::make_unique<PrintHelloAction>())
            .AddLeaf("Show Version", std::make_unique<ShowVersionAction>())
            .Parent()
        .AddBranch("Settings")
            .AddLeaf("Reset to Defaults", std::make_unique<PrintHelloAction>())
            .Parent()
        .Build();
}

// ----------------------------
// 8) Program entry
// ----------------------------
int main()
{
    auto menu = BuildMenu();
    StandardOutput output;
    StandardInput input;
    MenuRenderer renderer(output);
    MenuInput menuInput(input, renderer);
    MenuEngine engine(std::move(menu), renderer, menuInput);
    engine.Run();
    return 0;
}
