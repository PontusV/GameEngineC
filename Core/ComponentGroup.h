#ifndef COMPONENT_GROUP_H
#define COMPONENT_GROUP_H

#include "ComponentArrayManager.h"
#include "ComponentArray.h"
#include "ComponentTypeInfo.h"

#include <tuple>
#include <initializer_list>

namespace Core {
	/* Has access to all chunks compatible with its template. */
	template <typename T, typename... Ts>
	class ComponentGroup {
	public:
		ComponentGroup()
			: componentArrays{
				ComponentArrayManager::getInstance().createComponentArray<T>({ ComponentTypeInfo<T>::getType(), ComponentTypeInfo<Ts>::getType()... }, {}),
				ComponentArrayManager::getInstance().createComponentArray<Ts>({ ComponentTypeInfo<T>::getType(), ComponentTypeInfo<Ts>::getType()... }, {})...
			} {
		}
		ComponentGroup(std::initializer_list<ComponentType> filterTypes)
			: componentArrays{
				ComponentArrayManager::getInstance().createComponentArray<T>({ ComponentTypeInfo<T>::getType(), ComponentTypeInfo<Ts>::getType()... }, filterTypes),
				ComponentArrayManager::getInstance().createComponentArray<Ts>({ ComponentTypeInfo<T>::getType(), ComponentTypeInfo<Ts>::getType()... }, filterTypes)...
			} {
		}
		virtual ~ComponentGroup() {}

		template <typename Type>
		Type& get(std::size_t index) {
			return std::get<ComponentArray<Type>*>(componentArrays)->get(index);
		}

		template <typename Type>
		ComponentArray<Type>& getArray() {
			return *std::get<ComponentArray<Type>*>(componentArrays);
		}

		Entity& getEntity(std::size_t index) {
			return std::get<0>(componentArrays)->getEntity(index);
		}

		std::size_t size() {
			return std::get<0>(componentArrays)->size();
		}
	private:
		std::tuple<ComponentArray<T>*, ComponentArray<Ts>*...> componentArrays;
	};
}
#endif