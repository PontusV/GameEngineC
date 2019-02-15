#ifndef COMPONENT_GROUP_H
#define COMPONENT_GROUP_H

#include <tuple>
#include <vector>
#include <utility>
#include <initializer_list>
#include "ComponentArrayManager.h"
#include "ComponentArray.h"
#include "Component.h"

namespace GameEngine {
	/* Has access to all chunks compatible with its template. */
	template <typename... Ts>
	class ComponentGroup {
	public:
		ComponentGroup() : componentArrays({ ComponentArrayManager<Ts>::getInstance().createComponentArray({ Ts::TYPE_ID... })... }) {}
		ComponentGroup(std::initializer_list<ComponentTypeID> filterIDs) : componentArrays({ ComponentArrayManager<Ts>::getInstance().createComponentArray({ Ts::TYPE_ID... }, filterIDs)... }) {}
		virtual ~ComponentGroup() {}

		template <typename T>
		ComponentArray<T>& getComponentArray() {
			return std::get<ComponentArray<T>&>(componentArrays);
		}
	private:
		std::size_t length;
		std::tuple<ComponentArray<Ts>&...> componentArrays;
	};
}
#endif