#include "Layer.h"
using namespace Core;


Layer::Layer() : entityManager(std::make_shared<EntityManager>()) {}

Layer::~Layer() {}


void Layer::destroyEntity(Entity entity) {
	entityManager->destroyEntity(entity);
}