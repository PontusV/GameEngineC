#ifndef CHILD_MANAGER_H
#define CHILD_MANAGER_H

#include "Component.h"
#include "Entity.h"
#include "ChildListener.h"
#include <vector>
#include <cstddef>

namespace Core {

	class ChildManager : public Component, public ChildListener {
	public:
		static constexpr ComponentTypeID TYPE_ID = 7;
		const ComponentTypeID getTypeID() const override { return TYPE_ID; }

		ChildManager(std::istream& is);
		ChildManager() {}
		~ChildManager();

		void childAdded(ChunkEntityHandle entity);
		void childRemoved(Entity entity);

		std::size_t			getChildCount();
		ChunkEntityHandle*	getChild(std::size_t index);
		std::vector<ChunkEntityHandle>& getChildren();

		void registerListener(ChildListener* listener);
		void unregisterListener(ChildListener* listener);

		// Save & Load operator
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);

	private:
		std::vector<ChunkEntityHandle> children;
		std::vector<ChildListener*> listeners;
	};
}
#endif