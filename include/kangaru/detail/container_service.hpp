#pragma once

#include <tuple>
#include "utils.hpp"

namespace kgr {

struct Container;

namespace detail {
struct ContainerServiceBase{};

template<typename Original, typename Service>
struct ServiceOverride : Service {
    virtual ~ServiceOverride() {}
    ServiceOverride(Original& service) : _service{service} {}

    ServiceType<Service> forward() override {
        return static_cast<ServiceType<Service>>(_service.forward());
    }

private:
    Original& _service;
};

}

struct Single {
	Single() = default;
	virtual ~Single() = default;
	Single(const Single&) = delete;
	Single& operator=(const Single&) = delete;
	Single(Single&&) = default;
	Single& operator=(Single&&) = default;
};

template<typename... Types>
struct Overrides {
	using ParentTypes = std::tuple<Types...>;
};

struct ContainerService : detail::ContainerServiceBase {
	ContainerService(Container& instance) : _instance{instance} {}
	
	Container& forward() {
		return _instance;
	}
	
private:
	Container& _instance;
};

template<typename T>
struct DerivedContainerService : detail::ContainerServiceBase {
	using Type = T;
	DerivedContainerService(T& instance) : _instance{instance} {}
	
	T& forward() {
		return _instance;
	}
	
private:
	T& _instance;
};

}
