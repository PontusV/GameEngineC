#ifndef HIERARCHY_ORDER_RECT_H
#define HIERARCHY_ORDER_RECT_H
#include "entity/component/Component.h"
#include "entity/handle/EntityHandle.h"
#include <cstddef>
#include "HierarchyOrderRect.generated.h"
namespace Core {
	CLASS() HierarchyOrderRect : public Component {
		GENERATED_BODY()
	public:
		HierarchyOrderRect(EntityHandle entity, std::size_t order);
		~HierarchyOrderRect();

	public:
		EntityHandle entity;
		std::size_t order;
	};
}
#endif