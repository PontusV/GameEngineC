#ifndef REFLECTED_OBJECT_HANDLE_H
#define REFLECTED_OBJECT_HANDLE_H
#include "entity/handle/ComponentHandle.h"
#include <cstddef>
namespace Core {

	class ReflectedObjectHandle {
	public:
		ReflectedObjectHandle(ComponentHandle handle, std::size_t offset, std::size_t typeID) : handle(handle), offset(offset), typeID(typeID) {}
		~ReflectedObjectHandle() {}

		bool isValid();
		void* get();
		std::size_t getTypeID() const;

		Component* getRoot();
		std::size_t getRootPropTypeID();

	private:
		ComponentHandle handle;
		std::size_t offset;
		std::size_t typeID;
	};
}
#endif