#ifndef COMPONENT_FUNCTION_HANDLE_H
#define COMPONENT_FUNCTION_HANDLE_H
#include "ComponentHandle.h"
#include <memory>
namespace Core {
	class IComponentFunctionHandle {
	public:
		virtual ~IComponentFunctionHandle() {}
		virtual void call() = 0;
	};

	template<typename T>
	class ComponentFunctionHandle : public IComponentFunctionHandle {
	private:
		typedef void(T::*FunctionPtr)();
	public:
		ComponentFunctionHandle(ComponentHandle component, FunctionPtr ptr) : component(component), functionPtr(ptr) {}
		void call() {
			T* componentPtr = (T*)component.getComponent();
			if (componentPtr) // If the handle is still valid
				(componentPtr->*functionPtr)();
		}
	private:
		ComponentHandle component;
		FunctionPtr functionPtr;
	};

	template<typename T>
	ComponentFunctionHandle<T> bind(T* component, void(T::*ptr)()) {
		return ComponentFunctionHandle(ComponentHandle(typeIDof(T), component->getOwner()), ptr);
	}

	class ComponentFunctionHandleWrapper {
	public:
		ComponentFunctionHandleWrapper() {}
		~ComponentFunctionHandleWrapper() {}
		template<typename T>
		ComponentFunctionHandleWrapper(ComponentFunctionHandle<T> function) {
			this->function = std::make_shared<ComponentFunctionHandle<T>>(function);
		}

		template<typename T>
		ComponentFunctionHandleWrapper& operator=(ComponentFunctionHandle<T> function) {
			this->function = std::make_shared<ComponentFunctionHandle<T>>(function);
			return *this;
		}
		/* Calls the bound function. If none exists nothing happens. */
		void call() {
			if (function)
				function->call();
		}
	private:
		std::shared_ptr<IComponentFunctionHandle> function;
	};
}
#endif