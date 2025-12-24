#pragma once
#include <optional>
#include <string>
namespace utils
{
template <typename TData = void> struct ServiceResult
{
    bool success;
    std::string message;
    std::optional<TData> data;
    static ServiceResult Success(TData data, std::string message = "")
    {
        return {true, std::move(message), std::move(data)};
    }
    static ServiceResult Success(std::string message = "")
        requires std::is_void_v<TData>
    {
        return {true, std::move(message), std::nullopt};
    }
    static ServiceResult Failure(std::string message)
    {
        return {false, std::move(message), std::nullopt};
    }
};
template <> struct ServiceResult<void>
{
    bool success;
    std::string message;
    static ServiceResult Success(std::string message = "")
    {
        return {true, std::move(message)};
    }
    static ServiceResult Failure(std::string message)
    {
        return {false, std::move(message)};
    }
};
} // namespace utils