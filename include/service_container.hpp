#pragma once
#include "service_not_registered_exception.hpp"

#include <any>
#include <memory>
#include <typeindex>
#include <unordered_map>

class ServiceContainer
{
  public:
    ServiceContainer();
    template <typename T> void Register(std::shared_ptr<T> instance)
    {
        services_[std::type_index(typeid(T))] = instance;
    }
    template <typename T> std::shared_ptr<T> Resolve() const
    {
        auto it = services_.find(std::type_index(typeid(T)));
        if (it == services_.end())
        {
            throw ServiceNotRegisteredException(typeid(T).name());
        }
        return std::any_cast<std::shared_ptr<T>>(it->second);
    }

  private:
    std::unordered_map<std::type_index, std::any> services_;
};