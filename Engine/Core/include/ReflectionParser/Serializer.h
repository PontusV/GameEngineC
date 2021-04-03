#ifndef REFLECTION_SERIALIZER_H
#define REFLECTION_SERIALIZER_H

#include "ReflectionTypes.h"
#include <iostream>
#include <type_traits>
#include <string>
#include <vector>
#include <stdexcept>
#include <cassert>

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

	namespace details {
		//-----------------------------------------SERIALIZE-----------------------------------------------
		// Literal type
		template<typename T>
		static std::enable_if_t<std::is_arithmetic<T>::value && !std::is_array<T>::value, void> serialize(const T& value, std::ostream& os) {
			os.write((char*)&value, sizeof(T));
		}

		// Array of literal types
		template<typename T>
		static std::enable_if_t<std::is_arithmetic<T>::value&& std::is_array<T>::value, void> serialize(const T& value, std::ostream& os) {
			os.write((char*)&value, sizeof(T));
		}

		// Non literal type, assumed to have compatible serialize function
		template<typename T>
		static std::enable_if_t<!std::is_arithmetic<T>::value&& is_serializeable<T>::value, void> serialize(const T& value, std::ostream& os) {
			value.serialize(os);
		}

		template<typename T>
		static std::enable_if_t<!std::is_arithmetic<T>::value && !is_serializeable<T>::value, void> serialize(const T& value, std::ostream& os) {
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
		static std::enable_if_t<std::is_arithmetic<T>::value && !std::is_array<T>::value, void> deserialize(T& value, std::istream& is) {
			is.read((char*)&value, sizeof(T));
		}

		// Array of literal types
		template<typename T>
		static std::enable_if_t<std::is_arithmetic<T>::value&& std::is_array<T>::value, void> deserialize(T& value, std::istream& is) {
			is.read((char*)&value, sizeof(T));
		}

		// Non literal type, assumed to have compatible deserialize function
		template<typename T>
		static std::enable_if_t<!std::is_arithmetic<T>::value&& is_deserializeable<T>::value, void> deserialize(T& value, std::istream& is) {
			value.deserialize(is);
		}

		template<typename T>
		static std::enable_if_t<!std::is_arithmetic<T>::value && !is_deserializeable<T>::value, void> deserialize(T& value, std::istream& is) {
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
				is.read((char*)&wChar, sizeof(wchar_t));
				if (wChar == L'\0')
					break;
				else
					value = value + wChar;
			}
		}

		struct SerializedPropertyData {
			std::string name;
			std::string typeName;
			std::size_t typeSize;
		};

		template<std::size_t I = 0>
		static void serializePropertiesImpl(std::ostream& os, const std::vector<Property>& properties) {} // End

		template<std::size_t I = 0>
		static void deserializePropertiesImpl(std::istream& is, const std::vector<Property>& properties, SerializedPropertyData prop) {
			is.seekg(prop.typeSize, std::ios::cur); // Skip property
			std::cout << "Mirror::Serializer::Deserialize Skipping property: " << prop.name << std::endl;
		} // End

		template<std::size_t I = 0, typename T, typename... Ts>
		static void serializePropertiesImpl(std::ostream& os, const std::vector<Property>& properties, const T& arg, const Ts&... args) {
			details::serialize(properties[I].name, os);
			details::serialize(properties[I].type.name, os);
			details::serialize(properties[I].type.size, os);
			details::serialize(arg, os);
			serializePropertiesImpl<I + 1>(os, properties, args...);
		}

		template<std::size_t I = 0, typename T, typename... Ts>
		static void deserializePropertiesImpl(std::istream& is, const std::vector<Property>& properties, SerializedPropertyData prop, T& arg, Ts&... args) {
			if (properties[I].name.compare(prop.name) == 0 && properties[I].type.name.compare(prop.typeName) == 0 && properties[I].type.size == prop.typeSize) {
				// Found match
				details::deserialize(arg, is);
			}
			else {
				// Keep searching
				deserializePropertiesImpl<I + 1>(is, properties, prop, args...);
			}
		}
	}

	template<typename... Ts>
	static void serialize(std::ostream& os, Ts&... args) {
		(details::serialize(args, os), ...);
	}

	template<typename... Ts>
	static void deserialize(std::istream& is, Ts&... args) {
		(details::deserialize(args, is), ...);
	}

	template<typename... Ts>
	static void serializeProperties(std::ostream& os, const std::vector<Property>& properties, Ts&... args) {
		std::size_t size = properties.size();
		assert(sizeof...(args) == size);
		details::serialize(size, os);
		details::serializePropertiesImpl(os, properties, args...);
	}

	template<typename... Ts>
	static void deserializeProperties(std::istream& is, const std::vector<Property>& properties, Ts&... args) {
		std::size_t count = 0;
		details::deserialize(count, is);
		for (std::size_t i = 0; i < count; i++) {
			details::SerializedPropertyData prop;
			details::deserialize(prop.name, is);
			details::deserialize(prop.typeName, is);
			details::deserialize(prop.typeSize, is);
			details::deserializePropertiesImpl(is, properties, prop, args...);
		}
	}
}
#endif