#pragma once
#include <string>

class IOutput
{
public:
    virtual ~IOutput() = default;
    virtual void Write(const std::string &text) = 0;
    virtual void WriteLine(const std::string &text) = 0;
};

class IInput
{
public:
    virtual ~IInput() = default;
    virtual std::string ReadLine() = 0;
};

class StandardOutput : public IOutput
{
public:
    void Write(const std::string &text) override;
    void WriteLine(const std::string &text) override;
};

class StandardInput : public IInput
{
public:
    std::string ReadLine() override;
};
