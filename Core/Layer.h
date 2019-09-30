#ifndef LAYER_H
#define LAYER_H
#include "Level.h"
#include "EntityManager.h"
#include <vector>
#include <memory>
namespace Core {
	class Layer {
	public:
		Layer();
		~Layer();

		template <typename... Ts>
		EntityHandle createEntity(std::string name, Ts... components);
		void destroyEntity(Entity entity);

	private:
		std::vector<Level> scenes;
		std::shared_ptr<EntityManager> entityManager;
	};

	template <typename... Ts>
	EntityHandle Layer::createEntity(std::string name, Ts... components) {
		return entityManager->createEntity(name, components...);
	}
}
#endif