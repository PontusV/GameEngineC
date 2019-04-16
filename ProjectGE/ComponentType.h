#ifndef COMPONENT_TYPE_H
#define COMPONENT_TYPE_H

#include <vector>

namespace Core {

	typedef std::size_t ComponentTypeID;

	/* A collection of ComponentTypeIDs, all deriving from the type with typeID equal to the first entry. */
	class ComponentType {
	public:
		ComponentType(std::vector<ComponentTypeID> idList) : idList(idList) {}
		ComponentType() {}
		~ComponentType() {}

		bool operator==(const ComponentTypeID& other) const {
			for (const ComponentTypeID& id : idList) {
				if (id == other)
					return true;
			}
			return false;
		}

		ComponentTypeID getTypeID() const {
			return idList[0];
		}

		const std::vector<ComponentTypeID>& getDerivedList() const {
			return idList;
		}

	private:
		std::vector<ComponentTypeID> idList;
	};
}
bool operator==(const Core::ComponentType& left, const Core::ComponentType& right);
bool operator!=(const Core::ComponentType& left, const Core::ComponentType& right);
#endif