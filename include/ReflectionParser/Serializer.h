#ifndef REFLECTION_SERIALIZER_H
#define REFLECTION_SERIALIZER_H

#include "ReflectionTypes.h"
#include <iostream>
#include <type_traits>
#include <string>
#include <vector>
#include <stdexcept>

namespace Mirror {

	//SFINAE checks if the type has a member called serialize
	template<typename T>
	struct is_serializeable {
	private:
		template <typename C> static constexpr bool test(decltype(&C::serialize)) { return true; }
		template <typename C> static constexpr bool test(...) { return false; }

	public:
		static constexpr bool value = test<T>(0);
	};

	//SFINAE checks if the type has a member called deserialize
	template<typename T>
	struct is_deserializeable {
	private:
		template <typename C> static constexpr bool test(decltype(&C::deserialize)) { return true; }
		template <typename C> static constexpr bool test(...) { return false; }

	public:
		static constexpr bool value = test<T>(0);
	};

	//-----------------------------------------SERIALIZE-----------------------------------------------

	// Literal type
	template<typename T>
	static std::enable_if_t<std::is_literal_type<T>::value && !std::is_array<T>::value, void> serialize(const T& value, std::ostream& os) {
		os.write((char*)&value, sizeof(T));
	}

	// Array of literal types
	template<typename T>
	static std::enable_if_t<std::is_literal_type<T>::value && std::is_array<T>::value, void> serialize(const T& value, std::ostream& os) {
		os.write((char*)&value, sizeof(T));
	}

	// Non literal type, assumed to be Reflected
	template<typename T>
	static std::enable_if_t<!std::is_literal_type<T>::value && is_serializeable<T>::value, void> serialize(const T& value, std::ostream& os) {
		value.serialize(os);
	}

	template<typename T>
	static std::enable_if_t<!std::is_literal_type<T>::value && !is_serializeable<T>::value, void> serialize(const T& value, std::ostream& os) {
		throw std::invalid_argument("You need to implement \"void serialize(std::ostream&) const\" for this class!");
	}

	// std::vector
	template<typename T>
	static void serialize(const std::vector<T>& vector, std::ostream& os) {
		for (const T& value : vector) {
			serialize(value, os);
		}
	}

	// std::string
	template<>
	static void serialize<std::string>(const std::string& value, std::ostream& os) {
		os.write(value.c_str(), sizeof(char) * (value.size() + 1));
	}

	// std::wstring
	template<>
	static void serialize<std::wstring>(const std::wstring& value, std::ostream& os) {
		os.write((char*)value.c_str(), sizeof(wchar_t) * (value.size() + 1));
	}

	//---------------------------------------DESERIALIZE-----------------------------------------------
	// Literal type
	template<typename T>
	static std::enable_if_t<std::is_literal_type<T>::value && !std::is_array<T>::value, void> deserialize(T& value, std::istream& is) {
		is.read((char*)&value, sizeof(T));
	}

	// Array of literal types
	template<typename T>
	static std::enable_if_t<std::is_literal_type<T>::value && std::is_array<T>::value, void> deserialize(T& value, std::istream& is) {
		is.read((char*)& value, sizeof(T));
	}

	// Non literal type, assumed to be Reflected
	template<typename T>
	static std::enable_if_t<!std::is_literal_type<T>::value && is_deserializeable<T>::value, void> deserialize(T& value, std::istream& is) {
		value.deserialize(is);
	}

	template<typename T>
	static std::enable_if_t<!std::is_literal_type<T>::value && !is_deserializeable<T>::value, void> deserialize(T& value, std::istream& is) {
		throw std::invalid_argument("You need to implement \"void deserialize(std::istream&)\" for this class!");
	}

	// std::vector
	template<typename T>
	static void deserialize(std::vector<T>& vector, std::istream& is) {
		for (T& value : vector) {
			deserialize(value, is);
		}
	}

	// std::string
	template<>
	static void deserialize<std::string>(std::string& value, std::istream& is) {
		std::getline(is, value, '\0');
	}

	// std::wstring
	template<>
	static void deserialize<std::wstring>(std::wstring& value, std::istream& is) {
		while (!is.eof()) {
			wchar_t wChar;
			is.read((char*)& wChar, sizeof(wchar_t));
			if (wChar == L'\0')
				break;
			else
				value = value + wChar;
		}
	}
}
#endif