#ifndef COMPONENT_TYPE_H
#define COMPONENT_TYPE_H

#include <vector>
#include <stdexcept>

namespace Core {

	typedef std::size_t ComponentTypeID;

	/* A collection of ComponentTypeIDs, all deriving from the type with typeID equal to the first entry. */
	class ComponentType {
	public:
		ComponentType(std::vector<ComponentTypeID> idList) : idList(idList) {
		}
		ComponentType(ComponentTypeID typeID);
		ComponentType() {
		}
		~ComponentType() {}

		ComponentTypeID getTypeID() const {
			if (idList.empty()) throw std::invalid_argument("Cannot get typeID from invalid ComponentType");
			return idList[0];
		}

		const std::vector<ComponentTypeID>& getDerivedList() const {
			return idList;
		}

	private:
		std::vector<ComponentTypeID> idList;
	};
}

bool operator==(const Core::ComponentType& left, const Core::ComponentTypeID& right);
bool operator==(const Core::ComponentTypeID& left, const Core::ComponentType& right);
bool operator==(const Core::ComponentType& left, const Core::ComponentType& right);
bool operator!=(const Core::ComponentType& left, const Core::ComponentType& right);
#endif