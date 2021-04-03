#ifndef REFLECTION_UTILITY_H
#define REFLECTION_UTILITY_H

#include <vector>
#include <stdexcept>
#include <type_traits>


namespace Mirror {
	// Type Conversion helper for the generated headers
	template<typename To, typename From>
	std::enable_if_t<std::is_convertible<From, To>::value || std::is_same<From, To>::value, To> convertType(const From& from) {
		return static_cast<To>(from);
	}

	template<typename To, typename From>
	std::enable_if_t<!std::is_convertible<From, To>::value && !std::is_same<From, To>::value, To&> convertType(const From& from) {
		throw std::invalid_argument("Cannot convert type!\n");
	}

	// Array
	template<typename To, typename From, std::size_t N>
	std::enable_if_t<std::is_convertible<From, To>::value, std::vector<To>> convertArrayType(From from[N]) {
		// Array conversion for each element
		std::vector<To> result(N);
		for (std::size_t i = 0; i < N; i++) {
			result[i] = static_cast<To>(from[i]);
		}
		return result;
	}

	template<typename To, typename From, std::size_t N>
	std::enable_if_t<!std::is_convertible<From, To>::value, std::vector<To>> convertArrayType(From from[N]) {
		throw std::invalid_argument("Cannot convert type!\n");
	}

	// Set variable
	template<typename T, typename T2>
	void setVariableValue(T& var, const T2& value) {
		var = convertType<T>(value);
	}
}

#endif