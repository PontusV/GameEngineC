#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H
#include "Component.h"
#include "ObjectData.h"
#include "Scene.h"
#include "EntityHandle.h"
#include "Behaviour.generated.h"

namespace Core {

	class Input;
	class Window;
	class Camera;

	CLASS() Behaviour : public Component {
		GENERATED_BODY()
	public:
		virtual ~Behaviour() = 0; // Abstract

		/* Called when the Script is being loaded. */
		virtual void awake() {};
		/* Called when the Script is enabled. */
		virtual void start() {};
		/* Called every frame. */
		virtual void update(float deltaTime) {};
		/* Called every frame, after update has been called in all behaviours. */
		virtual void lateUpdate(float deltaTime) {};
		/* Called when this component is destroyed. */
		virtual void onDestroy() {};

		//
		virtual void onPreRender() {};
		virtual void onPostRender() {};

		// Input
		/* Called when a mouse button was pressed. */
		virtual void onMouseButtonPressed(int buttoncode, int mods) {}
		/* Called when a mouse button was released. */
		virtual void onMouseButtonReleased(int buttoncode, int mods) {}
		/* Called when a mouse button was pressed while the mouse points on the owner. */
		virtual void onMouseButtonPressedAsButton(int buttoncode, int mods) {}
		/* Called when a mouse button was released while the mouse points on the owner. */
		virtual void onMouseButtonReleasedAsButton(int buttoncode, int mods) {}
		/* Called when the mouse moves while the left mouse button is held down and the owner is the last Entity to have been clicked on. */
		virtual void onMouseDrag(float mouseX, float mouseY) {}
		/* Called when the owner was double clicked on. */
		virtual void onDoubleClick() {}
		/* Called when the mouse hovers over the owner. */
		virtual void onHoverover() {}
		/* Called when the mouse stops hovering over the owner. */
		virtual void onHoverout() {}
		/* Called when the mouse scroll is moved while the mouse points on the owner. */
		virtual void onScroll(float xoffset, float yoffset) {}

		virtual void onTransformChanged() {}

		// Hierarchy
		virtual void onChildAdded(EntityHandle entity) {}
		virtual void onChildRemoved(EntityHandle entity) {}
		/* Called when a child has been changed; either by having a component removed or added. */
		virtual void onChildChanged(EntityHandle entity) {}
		/* WIP. Called when another parent has been set or if the parent was lost. */
		virtual void onParentChanged() {}

		void enable();
		void disable();

	public:
		bool started = false;
		bool enabled = false;
		bool runInEditMode = true;

		static Input* input;
		static Window* window;
		static Camera* camera;
	protected:
		Behaviour();

		/* Creates an Entity in the same Scene at the end of the frame. */
		template<typename... Ts>
		EntityHandle createEntity(std::string name, Ts... components);

		void destroyEntity(const EntityHandle& handle);
		void destroyEntity(Entity entity);
	};

	template<typename... Ts>
	EntityHandle Behaviour::createEntity(std::string name, Ts... components) {
		Scene* scene = owner.getComponent<ObjectData>()->getScene();
		return scene->createEntityQueued(name, components...);
	}
}
#endif