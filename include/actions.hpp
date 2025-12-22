#pragma once

struct IMenuAction
{
    virtual ~IMenuAction() = default;
    virtual void Execute() = 0;
};

struct PrintHelloAction final : IMenuAction
{
    void Execute() override;
};

struct ShowVersionAction final : IMenuAction
{
    void Execute() override;
};
