#ifndef PARENT_ENTITY_H
#define PARENT_ENTITY_H

#include "Component.h"
#include "Entity.h"

namespace Core {

	class ParentEntity : public Component {
	public:
		static constexpr ComponentTypeID TYPE_ID = 6;
		const ComponentTypeID getTypeID() const override { return TYPE_ID; }

		ParentEntity(Entity parent);
		ParentEntity(std::istream& is);
		ParentEntity() {}
		~ParentEntity();

		void	setParent(Entity parent);
		Entity	getParent() const;

		// Save & Load operator
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);
	private:
		Entity parent;
	};
}
#endif