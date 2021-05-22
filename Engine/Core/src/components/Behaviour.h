#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "entity/component/Component.h"
#include "entity/handle/EntityHandle.h"
#include "scene/Scene.h"
#include "Behaviour.generated.h"

namespace Core {

	class Input;
	class Window;
	class Camera;
	class SceneManager;

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
		/* Called when this component becomes enabled and active. */
		virtual void onEnable() {};
		/* Called when this component becomes disabled and active. */
		virtual void onDisable() {};

		//
		virtual void onPreRender(float deltaTime) {};
		virtual void onPostRender(float deltaTime) {};

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
		virtual void onHoverOver() {}
		/* Called when the mouse stops hovering over the owner. */
		virtual void onHoverOut() {}
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
		bool isEnabled();
		bool isActive();
		void activate();

	private:
		bool enabled = false;
		bool active = false;
	public:
		bool started = false;
		bool runInEditMode = false;

		static Input* input;
		static Window* window;
		static Camera* camera;
		static SceneManager* sceneManager;
	protected:
		Behaviour();

		/* Creates an Entity in the same Scene at the end of the frame. */
		template<typename... Ts>
		EntityHandle createEntity(std::string name, Ts... components);

		/* Destroys the Entity in its own scene. */
		void destroyEntity(EntityHandle& handle);
		/* Destroys the Entity in this Scene. */
		void destroyEntity(Entity entity);
	};

	template<typename... Ts>
	EntityHandle Behaviour::createEntity(std::string name, Ts... components) {
		Scene* scene = owner.getScene();
		return scene->createEntityQueued(name, components...);
	}
}
#endif