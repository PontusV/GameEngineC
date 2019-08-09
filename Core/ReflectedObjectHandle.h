#ifndef REFLECTED_OBJECT_HANDLE_H
#define REFLECTED_OBJECT_HANDLE_H
#include "ComponentHandle.h"
#include <cstddef>
namespace Core {
	class ReflectedObject;

	class ReflectedObjectHandle {
	public:
		ReflectedObjectHandle(ComponentHandle handle, std::size_t offset) : handle(handle), offset(offset) {}
		~ReflectedObjectHandle() {}

		bool isValid();
		ReflectedObject* get();

	private:
		ComponentHandle handle;
		std::size_t offset;
	};
}
#endif