#ifndef COMPONENT_FUNCTION_HANDLE_H
#define COMPONENT_FUNCTION_HANDLE_H
#include "ComponentHandle.h"
#include <functional>
#include <memory>
namespace Core {
	template<typename... Args>
	class IComponentFunctionHandleImpl {
	public:
		virtual ~IComponentFunctionHandleImpl() {}
		virtual void call(Args... args) = 0;
	};

	template<typename T, typename R, typename... Args>
	class ComponentFunctionHandleImpl : public IComponentFunctionHandleImpl<Args...> {
	private:
		typedef R(T::*FunctionPtr)(Args...);
	public:
		ComponentFunctionHandleImpl(ComponentHandle component, FunctionPtr ptr) : component(component), functionPtr(ptr) {}
		void call(Args... args) {
			T* componentPtr = (T*)component.getComponent();
			if (componentPtr) // If the handle is still valid
				(componentPtr->*functionPtr)(args...);
		}
	private:
		ComponentHandle component;
		FunctionPtr functionPtr;
	};

	template<typename T, typename R, typename... Args>
	ComponentFunctionHandleImpl<T, R, Args...> bind(T* component, R(T::*ptr)(Args...)) {
		return ComponentFunctionHandleImpl(ComponentHandle(typeIDof(T), component->getOwner()), ptr);
	}

	template<typename R, typename... Args>
	class ComponentFunctionHandle {
	public:
		ComponentFunctionHandle() {}
		~ComponentFunctionHandle() {}
		template<typename T>
		ComponentFunctionHandle(ComponentFunctionHandleImpl<T, R, Args...> function) {
			this->function = std::make_shared<ComponentFunctionHandleImpl<T, R, Args...>>(function);
		}

		template<typename T>
		ComponentFunctionHandle& operator=(ComponentFunctionHandleImpl<T, R, Args...> function) {
			this->function = std::make_shared<ComponentFunctionHandleImpl<T, R, Args...>>(function);
			return *this;
		}
		/* Calls the bound function. If none exists nothing happens. */
		void call(Args... args) {
			if (function)
				function->call(args...);
		}
		bool isNull() {
			return function == nullptr;
		}
	private:
		std::shared_ptr<IComponentFunctionHandleImpl<Args...>> function;
	};
}
#endif