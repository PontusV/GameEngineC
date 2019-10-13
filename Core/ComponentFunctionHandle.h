#ifndef COMPONENT_FUNCTION_HANDLE_H
#define COMPONENT_FUNCTION_HANDLE_H
#include "ComponentHandle.h"
#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
namespace Core {
	template<typename... Args>
	class IComponentFunctionHandleImpl {
	protected:
		IComponentFunctionHandleImpl() {}
	public:
		virtual ~IComponentFunctionHandleImpl() {}
		virtual void invoke(Args... args) = 0;
	};

	template<typename T, typename R, typename... Args>
	class ComponentFunctionHandleImpl : public IComponentFunctionHandleImpl<Args...> {
	private:
		typedef R(T::* FunctionPtr)(Args...);
	public:
		ComponentFunctionHandleImpl(ComponentHandle component, FunctionPtr ptr) : component(component), functionPtr(ptr) {}
		void invoke(Args... args) {
			T* componentPtr = (T*)component.getComponent();
			if (componentPtr) // If the handle is still valid
				(componentPtr->*functionPtr)(args...);
		}
	private:
		ComponentHandle component;
		FunctionPtr functionPtr;
	};

	template<typename T, typename R, typename... Args>
	class ComponentFunctionHandleWrapper : public IComponentFunctionHandleImpl<> {
	private:
		typedef R(T::* FunctionPtr)(Args...);
	public:
		ComponentFunctionHandleWrapper(ComponentHandle component, FunctionPtr ptr, Args... args) : component(component), functionPtr(ptr), args(std::make_tuple(args...)) {}
		void invoke() {
			T* componentPtr = (T*)component.getComponent();
			if (componentPtr) // If the handle is still valid
				(componentPtr->*functionPtr)(std::get<Args>(args)...);
		}
	private:
		ComponentHandle component;
		FunctionPtr functionPtr;
		std::shared_ptr<ComponentFunctionHandleImpl<T, R, Args...>> function;
		std::tuple<typename std::remove_reference<Args>::type...> args;
	};

	template<typename T, typename R, typename... Args>
	std::shared_ptr<IComponentFunctionHandleImpl<Args...>> bind(T* component, R(T::*ptr)(Args...)) {
		return std::make_shared<ComponentFunctionHandleImpl<T, R, Args...>>(ComponentFunctionHandleImpl(ComponentHandle(component->getComponentID(), component->getOwner()), ptr));
	}

	template<typename T, typename R, typename Arg, typename... Args>
	std::shared_ptr<IComponentFunctionHandleImpl<>> bind(T* component, R(T::* ptr)(Arg, Args...), Arg arg, Args... args) {
		return std::make_shared<ComponentFunctionHandleWrapper<T, R, Arg, Args...>>(ComponentFunctionHandleWrapper(ComponentHandle(component->getComponentID(), component->getOwner()), ptr, arg, args...));
	}

	template<typename R, typename... Args>
	class ComponentFunctionHandle {
	public:
		ComponentFunctionHandle() {}
		ComponentFunctionHandle(std::shared_ptr<IComponentFunctionHandleImpl<Args...>> function) : function(function) {
		}
		~ComponentFunctionHandle() {}

		ComponentFunctionHandle& operator=(std::shared_ptr<IComponentFunctionHandleImpl<Args...>> function) {
			this->function = function;
			return *this;
		}

		/* Calls the bound function. If none exists nothing happens. */
		void invoke(Args... args) {
			if (function)
				function->invoke(args...);
		}
		bool isNull() {
			return function == nullptr;
		}
	private:
		std::shared_ptr<IComponentFunctionHandleImpl<Args...>> function;
	};
}
#endif