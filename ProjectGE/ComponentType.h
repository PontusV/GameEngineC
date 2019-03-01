#ifndef COMPONENT_TYPE_H
#define COMPONENT_TYPE_H
#include "TypeList.h"
#include <cstddef>
#include <stdexcept>
#include <tuple>
#include <iostream>

typedef std::size_t ComponentTypeID;
#define MAX_TYPES SIZE_MAX


namespace GameEngine {
	/* Used to hold different Types of Component. */
	template <typename T>
	struct TypeHolder {
		typedef T type;
	};


	namespace ComponentTypeHelper {
		template <std::size_t I = 0>
		constexpr std::size_t getComponentTypeIndex(TypeList<>, const ComponentTypeID typeID) {
			return MAX_TYPES;
		}

		template <std::size_t I = 0, typename T, typename... Ts>
		constexpr std::size_t getComponentTypeIndex(TypeList<T, Ts...>, const ComponentTypeID typeID) {
			if (typeID == T::TYPE_ID)
				return I;

			return getComponentTypeIndex<I + 1>(TypeList<Ts...>(), typeID);
		}

		template <std::size_t N, typename... Ts>
		constexpr auto getComponentType(TypeList<Ts...>) {
			return TypeHolder<std::tuple_element<N, std::tuple<Ts...>>::type>();
		}
	}

	template <ComponentTypeID typeID>
	constexpr auto getComponentType() {
		constexpr TYPE_LIST typeList;
		//constexpr TypeList<Transform, Image, Text> typeList;
		constexpr std::size_t I = ComponentTypeHelper::getComponentTypeIndex(typeList, typeID);

		if (I == MAX_TYPES)
			throw std::invalid_argument("UNKOWN TYPE: " + typeID);

		return ComponentTypeHelper::getComponentType<I>(typeList);
	}

	template <typename... Ts>
	constexpr std::size_t getTypeAmount(TypeList<Ts...>) {
		return sizeof...(Ts);
	}

	constexpr std::size_t getTypeAmount() {
		constexpr TYPE_LIST typeList;
		return getTypeAmount(typeList);
	}
}
#endif