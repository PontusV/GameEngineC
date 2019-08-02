#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H
#include "Component.h"

namespace Core {

	class Input;
	class Window;

	#include "Behaviour.generated.h"

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
		virtual void onChildAdded(Handle entity) {}
		virtual void onChildRemoved(Handle entity) {}
		/* Called when a child has been changed; either by having a component removed or added. */
		virtual void onChildChanged(Handle entity) {}
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
	protected:
		Behaviour();

		/* Creates an Entity at the end of the frame. */
		template<typename... Ts>
		EntityHandle createEntity(std::string name, Ts... components);
	};

	template<typename... Ts>
	EntityHandle Behaviour::createEntity(std::string name, Ts... components) {
		return owner.createEntity(name, components...);
	}
}
#endif