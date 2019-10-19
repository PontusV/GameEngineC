#ifndef FUNCTION_CALLER_H
#define FUNCTION_CALLER_H

#include <functional>
#include <tuple>
#include <type_traits>
#include <vector>

namespace Core {
	class IFunctionCaller {
	public:
		virtual ~IFunctionCaller() = 0;
		virtual void call() = 0;
		virtual std::vector<void*> getArgumentPointers() = 0;
	};

	/* Stores a pointer to a member function and the arguments required to call the function. */
	template<typename R, typename Owner, typename... Args>
	class FunctionCaller : public IFunctionCaller {
	public:
		typedef R(Owner::*FunctionPtr)(Args...);
		FunctionCaller(FunctionPtr functionPtr, Owner& owner, Args... args) : functionPtr(functionPtr), arguments(std::make_tuple(std::ref(owner), args...)) {}
		~FunctionCaller() {}
		void call() {
			std::apply(functionPtr, arguments);
		}
		template<typename T>
		T* getArgument() {
			return &std::get<T>(arguments);
		}
		std::vector<void*> getArgumentPointers() {
			return getArgumentPointersImpl(std::index_sequence_for<Args...>{});
		}
	private:
		template<std::size_t... I>
		std::vector<void*> getArgumentPointersImpl(std::index_sequence<I...>) {
			return std::vector<void*>{ static_cast<void*>(&std::get<I + 1>(arguments))... };
		}
	private:
		FunctionPtr functionPtr;
		std::tuple<Owner&, typename std::remove_reference<Args>::type...> arguments;
	};
}
#endif