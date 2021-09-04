#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "entity/Entity.h"
#include "entity/handle/ComponentHandle.h"
#include "ReflectionPolymorph.generated.h"

#include <ReflectionParser/ReflectionTypes.h>
#include <cstddef>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <stdexcept>
#include <sstream>
#include <cassert>
#include <fstream>

namespace Core {

	typedef std::map<std::string, std::size_t> EntityRemapLoadInfo;
	typedef std::map<std::size_t, std::string> EntityRemapSaveInfo;

	template<typename, typename T>
	struct has_serialize {
		static_assert(
			std::integral_constant<T, false>::value,
			"Second template parameter needs to be of function type.");
	};

	template<typename, typename T>
	struct has_deserialize {
		static_assert(
			std::integral_constant<T, false>::value,
			"Second template parameter needs to be of function type.");
	};

	//SFINAE checks if the type has a member called serialize
	template<typename C, typename Ret, typename... Args>
	struct has_serialize<C, Ret(Args...)> {
	private:
		template<typename T>
		static constexpr auto check(T*) -> typename std::is_same<decltype(std::declval<T>().serialize(std::declval<Args>()...)), Ret>::type;

		template<typename>
		static constexpr std::false_type check(...);

		typedef decltype(check<C>(0)) type;

	public:
		static constexpr bool value = type::value;
	};

	//SFINAE checks if the type has a member called deserialize
	template<typename C, typename Ret, typename... Args>
	struct has_deserialize<C, Ret(Args...)> {
	private:
		template<typename T>
		static constexpr auto check(T*) -> typename std::is_same<decltype(std::declval<T>().deserialize(std::declval<Args>()...)), Ret>::type;

		template<typename>
		static constexpr std::false_type check(...);

		typedef decltype(check<C>(0)) type;

	public:
		static constexpr bool value = type::value;
	};

	template<typename T, typename Archive>
	struct is_serializeable {
		static constexpr bool value = has_serialize<T, void(Archive&)>::value;
	};

	template<typename T, typename Archive>
	struct is_deserializeable {
		static constexpr bool value = has_deserialize<T, void(Archive&)>::value;
	};

	namespace details {
		struct SerializationDetails {
			SerializationDetails(std::ostream& os, const EntityRemapSaveInfo* entityRemapInfo = nullptr) : os(os), entityRemapInfo(entityRemapInfo) {}
			std::ostream& os;
			const EntityRemapSaveInfo* entityRemapInfo;
		};

		struct DeserializationDetails {
			DeserializationDetails(std::istream& is, const EntityRemapLoadInfo* entityRemapInfo = nullptr) : is(is), entityRemapInfo(entityRemapInfo) {}
			std::istream& is;
			const EntityRemapLoadInfo* entityRemapInfo;
		};

		//-----------------------------------------SERIALIZE-----------------------------------------------
		template<typename T>
		std::enable_if_t<!std::is_arithmetic<T>::value, void> serialize(const T&, SerializationDetails& details) {
			std::cout << "Serializer::serialize::ERROR Failed to serialize given value type" << std::endl;
			throw std::invalid_argument("Failed to serialize given value type");
		}

		// Arithmetic type
		template<typename T>
		std::enable_if_t<std::is_arithmetic<T>::value && !std::is_array<T>::value, void> serialize(const T& value, SerializationDetails& details) {
			details.os.write((char*)&value, sizeof(T));
		}

		// Array of Arithmetic types
		template<typename T>
		std::enable_if_t<std::is_arithmetic<T>::value&& std::is_array<T>::value, void> serialize(const T& value, SerializationDetails& details) {
			details.os.write((char*)&value, sizeof(T));
		}

		// std::string
		template<>
		void serialize<std::string>(const std::string& value, SerializationDetails& details);

		// std::wstring
		template<>
		void serialize<std::wstring>(const std::wstring& value, SerializationDetails& details);

		// Mirror::Property (Note: Only saves part of the property)
		template<>
		void serialize<Mirror::Property>(const Mirror::Property& value, SerializationDetails& details);

		// Entity
		template<>
		void serialize<Entity>(const Entity& value, SerializationDetails& details);

		// ComponentHandle
		template<>
		void serialize<ComponentHandle>(const ComponentHandle& value, SerializationDetails& details);

		// Non literal type, assumed to have compatible serialize function
		template<typename T, typename Archive>
		std::enable_if_t<is_serializeable<T, Archive>::value, void> serialize(const T& value, Archive& ar) {
			value.serialize(ar);
		}

		template<typename T, typename Archive>
		std::enable_if_t<!is_serializeable<T, Archive>::value, void> serialize(const T& value, Archive& ar) {
			SerializationDetails details = ar.getDetails();
			serialize(value, details);
		}

		// std::pair
		template<typename T, typename T2, typename Archive>
		void serialize(const std::pair<T, T2>& value, Archive& ar) {
			serialize(value.first, ar);
			serialize(value.second, ar);
		}

		// std::map
		template<typename T, typename T2, typename Archive>
		void serialize(const std::map<T, T2>& value, Archive& ar) {
			std::size_t size = value.size();
			serialize(size, ar);
			for (const auto& pair : value) {
				serialize(pair, ar);
			}
		}

		// std::array
		template<typename T, std::size_t N, typename Archive>
		void serialize(const std::array<T, N>& array, Archive& ar) {
			serialize(N, ar);
			for (const T& value : array) {
				serialize(value, ar);
			}
		}

		// std::vector
		template<typename T, typename Archive>
		void serialize(const std::vector<T>& vector, Archive& ar) {
			std::size_t size = vector.size();
			serialize(size, ar);
			for (const T& value : vector) {
				serialize(value, ar);
			}
		}

		//---------------------------------------DESERIALIZE-----------------------------------------------

		template<typename T>
		std::enable_if_t<!std::is_arithmetic<T>::value, void> deserialize(T&, DeserializationDetails& details) {
			std::cout << "Serializer::deserialize::ERROR Failed to deserialize given value type" << std::endl;
			throw std::invalid_argument("Failed to deserialize given value type");
		}

		// Arithmetic type
		template<typename T>
		std::enable_if_t<std::is_arithmetic<T>::value && !std::is_array<T>::value, void> deserialize(T& value, DeserializationDetails& details) {
			details.is.read((char*)&value, sizeof(T));
		}

		// Array of Arithmetic types
		template<typename T>
		std::enable_if_t<std::is_arithmetic<T>::value && std::is_array<T>::value, void> deserialize(T& value, DeserializationDetails& details) {
			details.is.read((char*)&value, sizeof(T));
		}

		// std::string
		template<>
		void deserialize<std::string>(std::string& value, DeserializationDetails& details);

		// std::wstring
		template<>
		void deserialize<std::wstring>(std::wstring& value, DeserializationDetails& details);

		// Mirror::Property (Note: Only loads part of the property)
		template<>
		void deserialize<Mirror::Property>(Mirror::Property& value, DeserializationDetails& details);

		// Entity
		template<>
		void deserialize<Entity>(Entity& value, DeserializationDetails& details);

		// ComponentHandle
		template<>
		void deserialize<ComponentHandle>(ComponentHandle& value, DeserializationDetails& details);

		// Non literal type, assumed to have compatible deserialize function
		template<typename T, typename Archive>
		std::enable_if_t<is_deserializeable<T, Archive>::value, void> deserialize(T& value, Archive& ar) {
			value.deserialize(ar);
		}

		template<typename T, typename Archive>
		std::enable_if_t<!is_deserializeable<T, Archive>::value, void> deserialize(T& value, Archive& ar) {
			DeserializationDetails details = ar.getDetails();
			deserialize(value, details);
		}

		// std::pair
		template<typename T, typename T2, typename Archive>
		void deserialize(std::pair<T, T2>& value, Archive& ar) {
			deserialize(value.first, ar);
			deserialize(value.second, ar);
		}

		// std::map
		template<typename T, typename T2, typename Archive>
		void deserialize(std::map<T, T2>& value, Archive& ar) {
			std::size_t mapSize;
			deserialize(mapSize, ar);
			for (std::size_t i = 0; i < mapSize; i++) {
				std::pair<T, T2> pair;
				deserialize(pair, ar);
				value.insert(pair);
			}
		}

		// std::array
		template<typename T, std::size_t N, typename Archive>
		void deserialize(std::array<T, N>& array, Archive& ar) {
			std::size_t arraySize;
			deserialize(arraySize, ar);
			for (std::size_t i = 0; i < arraySize; i++) {
				if (i < N) {
					deserialize(array[i], ar);
				}
				else {
					// TODO: Skip
					T value;
					deserialize(value, ar);
				}
			}
		}

		// std::vector
		template<typename T, typename Archive>
		void deserialize(std::vector<T>& vector, Archive& ar) {
			std::size_t arraySize;
			deserialize(arraySize, ar);
			for (std::size_t i = 0; i < arraySize; i++) {
				deserialize(vector[i], ar);
			}
		}

		//---------------------------------------END OF DESERIALIZE-----------------------------------------------

		struct SerializedPropertyDetails {
			std::string name;		// Name of the property
			std::string typeName;	// Name of the type
			std::size_t typeSize;	// sizeof of the type
			std::size_t dataSize;	// Size of data in bytes when serialized
		};

		template<std::size_t I = 0, typename Archive>
		void serializePropertiesImpl(Archive& ar, const std::vector<Mirror::Property>& properties) {} // End

		template<std::size_t I = 0, typename Archive>
		void deserializePropertiesImpl(Archive& ar, const std::vector<Mirror::Property>& properties, SerializedPropertyDetails prop) {
			auto details = ar.getDetails();
			details.is.seekg(prop.dataSize, std::ios::cur); // Skip property
			std::cout << "Serializer::Deserialize Skipping property: " << prop.name << std::endl;
		} // End

		template<std::size_t I = 0, typename Archive, typename T, typename... Ts>
		void serializePropertiesImpl(Archive& ar, const std::vector<Mirror::Property>& properties, const T& arg, const Ts&... args) {
			details::serialize(properties[I].name, ar);
			details::serialize(properties[I].type.name, ar);
			details::serialize(properties[I].type.size, ar);
			SerializationDetails details = ar.getDetails();
			std::ostringstream dataBuffer;
			Archive bufferArchive = Archive(dataBuffer);
			if (details.entityRemapInfo) {
				bufferArchive.setEntityRemapInfo(*details.entityRemapInfo);
			}
			std::cout << "Serializing " << properties[I].name << " of type " << properties[I].type.name << std::endl;
			details::serialize(arg, bufferArchive);
			std::size_t dataSize = dataBuffer.tellp();
			details::serialize(dataSize, ar);
			details.os.write(dataBuffer.str().c_str(), dataSize);
			serializePropertiesImpl<I + 1>(ar, properties, args...);
		}

		template<std::size_t I = 0, typename Archive, typename T, typename... Ts>
		void deserializePropertiesImpl(Archive& ar, const std::vector<Mirror::Property>& properties, SerializedPropertyDetails prop, T& arg, Ts&... args) {
			if (properties[I].name.compare(prop.name) == 0 && properties[I].type.name.compare(prop.typeName) == 0 && properties[I].type.size == prop.typeSize) {
				// Found match
				std::cout << "Deserializing " << prop.name << " of type " << prop.typeName << std::endl;
				details::deserialize(arg, ar);
			}
			else {
				// Keep searching
				deserializePropertiesImpl<I + 1>(ar, properties, prop, args...);
			}
		}

		void skipPropertiesImpl(DeserializationDetails& details);

		template<typename... Ts, typename Archive>
		void serializeAll(Archive& ar, Ts&... args) {
			(details::serialize(args, ar), ...);
		}

		template<typename... Ts, typename Archive>
		void deserializeAll(Archive& ar, Ts&... args) {
			(details::deserialize(args, ar), ...);
		}

		template<typename Archive, typename... Ts>
		void serializeProperties(Archive& ar, const std::vector<Mirror::Property>& properties, Ts&... args) {
			std::size_t size = properties.size();
			assert(sizeof...(args) == size);
			details::serialize(size, ar);
			details::serializePropertiesImpl(ar, properties, args...);
		}

		template<typename Archive, typename... Ts>
		void deserializeProperties(Archive& ar, const std::vector<Mirror::Property>& properties, Ts&... args) {
			assert(sizeof...(args) == properties.size());
			std::size_t count = 0;
			details::deserialize(count, ar);
			for (std::size_t i = 0; i < count; i++) {
				details::SerializedPropertyDetails prop;
				details::deserialize(prop.name, ar);
				details::deserialize(prop.typeName, ar);
				details::deserialize(prop.typeSize, ar);
				details::deserialize(prop.dataSize, ar);
				details::deserializePropertiesImpl(ar, properties, prop, args...);
			}
		}

		/* Expects the upcoming part of the given istream to be serialized properties for a component. The skip will chain down to all base its classes */
		void skipProperties(details::DeserializationDetails& details);
	}

	class SerializerArchive {
	public:
		SerializerArchive(std::ostream& os) : os(os) {}
		~SerializerArchive() {}
		void setEntityRemapInfo(EntityRemapSaveInfo entityRemapInfo) {
			this->entityRemapInfo = entityRemapInfo;
		}
		EntityRemapSaveInfo getEntityRemapInfo() {
			return entityRemapInfo;
		}
		details::SerializationDetails getDetails() {
			return details::SerializationDetails(os, &entityRemapInfo);
		}
		template<typename... Ts>
		void operator()(Ts&... args) {
			details::serializeAll(*this, args...);
		}
		template<typename... Ts>
		void operator()(std::vector<Mirror::Property> properties, Ts&... args) {
			details::serializeProperties(*this, properties, args...);
		}
	private:
		EntityRemapSaveInfo entityRemapInfo;
		std::ostream& os;
	};

	class DeserializerArchive {
	public:
		DeserializerArchive(std::istream& is) : is(is) {}
		virtual ~DeserializerArchive() {}
		void setEntityRemapInfo(EntityRemapLoadInfo entityRemapInfo) {
			this->entityRemapInfo = entityRemapInfo;
		}
		EntityRemapLoadInfo getEntityRemapInfo() {
			return entityRemapInfo;
		}
		details::DeserializationDetails getDetails() {
			return details::DeserializationDetails(is, &entityRemapInfo);
		}
		void resetToStart() {
			is.clear();
			is.seekg(0);
		}
		template<typename... Ts>
		void operator()(Ts&... args) {
			details::deserializeAll(*this, args...);
		}
		template<typename... Ts>
		void operator()(std::vector<Mirror::Property>& properties, Ts&... args) {
			details::deserializeProperties(*this, properties, args...);
		}
		/* Expects the upcoming part of the given istream to be serialized properties for a component. The skip will chain down to all base its classes */
		void skipProperties() {
			auto details = getDetails();
			details::skipProperties(details);
		}
	protected:
		EntityRemapLoadInfo entityRemapInfo;
		std::istream& is;
	};

	class PrefabDeserializerArchive : public DeserializerArchive {
	public:
		PrefabDeserializerArchive(std::istream& is) : DeserializerArchive(is) {}
		~PrefabDeserializerArchive() {}
		void setOverridenProperties(std::vector<std::string> properties) {
			overridenProperties = properties;
		}
		template<typename... Ts>
		void operator()(Ts&... args) {
			details::deserializeAll(*this, args...);
		}
		template<typename... Ts>
		void operator()(const std::vector<Mirror::Property>& properties, Ts&... args) {
			std::vector<Mirror::Property> filteredProperties;
			for (const Mirror::Property& prop : properties) {
				if (std::find(overridenProperties.begin(), overridenProperties.end(), prop.name == overridenProperties.end())) {
					filteredProperties.push_back(prop);
				}
			}
			details::deserializeProperties(*this, filteredProperties, args...);
		}
	private:
		std::vector<std::string> overridenProperties;
	};
}
#endif