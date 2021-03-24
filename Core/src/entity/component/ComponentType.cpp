#include "ComponentType.h"
#include "ReflectionPolymorph.generated.h"
using namespace Core;

bool operator==(const ComponentType& left, const ComponentType& right) {
	for (const ComponentTypeID& id : left.getDerivedList()) {
		for (const ComponentTypeID& otherID : right.getDerivedList()) {
			if (id == otherID)
				return true;
		}
	}
	return false;
	//return left == right.getTypeID() || right == left.getTypeID();
}

bool operator!=(const ComponentType& left, const ComponentType& right) {
	return !(left == right);
}

bool operator==(const ComponentType& left, const ComponentTypeID& right) {
	for (const ComponentTypeID& id : left.getDerivedList()) {
		if (id == right)
			return true;
	}
	return false;
}

bool operator==(const ComponentTypeID& left, const ComponentType& right) {
	for (const ComponentTypeID& id : right.getDerivedList()) {
		if (id == left)
			return true;
	}
	return false;
}


ComponentType::ComponentType(ComponentTypeID typeID) {
	idList = Mirror::polyGetDerivedTypeIDs(typeID);
	idList.insert(idList.begin(), typeID);
}