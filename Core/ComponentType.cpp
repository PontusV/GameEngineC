#include "ComponentType.h"
using namespace Core;

bool operator==(const Core::ComponentType& left, const Core::ComponentType& right) {
	for (const ComponentTypeID& id : left.getDerivedList()) {
		for (const ComponentTypeID& otherID : right.getDerivedList()) {
			if (id == otherID)
				return true;
		}
	}
	return false;
}

bool operator!=(const Core::ComponentType& left, const Core::ComponentType& right) {
	return !(left == right);
}