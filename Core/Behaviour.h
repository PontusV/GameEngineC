#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H
#include "Component.h"

namespace Core {

	class Input;
	class Window;

	#include "Behaviour.Behaviour.generated.h"

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
		/* Called when this component is destroyed. */
		virtual void onDestroy() {};

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

		// Hierarchy
		/* Called when a new child has been added, moved in the hierarchy or removed. */
		virtual void onChildrenChanged() {}
		/* Called when another parent has been set or if the parent was lost. */
		virtual void onParentChanged() {}

		void enable();
		void disable();

	public:
		bool started;
		bool enabled;
		bool runInEditorMode = true;

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