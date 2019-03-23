#ifndef CHILD_LISTENER_H
#define CHILD_LISTENER_H

#include "ChunkEntityHandle.h"
#include "Entity.h"
namespace Core {
	class ChildListener {
	public:
		virtual ~ChildListener() {};
		virtual void childAdded(ChunkEntityHandle entity) = 0;
		virtual void childRemoved(Entity entity) = 0;
	};
}
#endif