#ifndef FUNCTION_CALLER_H
#define FUNCTION_CALLER_H

#include <functional>
#include <tuple>
#include <type_traits>

namespace Core {
	class IFunctionCaller {
	public:
		virtual ~IFunctionCaller() = 0;
		virtual void call() = 0;
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
	private:
		FunctionPtr functionPtr;
		std::tuple<Owner&, typename std::remove_reference<Args>::type...> arguments;
	};
}
#endif