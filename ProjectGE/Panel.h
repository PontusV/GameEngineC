#ifndef PANEL_H
#define PANEL_H
#include "BoxComponent.h"
#include "ChildCollection.h"
#include "GraphicComponent.h"

namespace Core {

	class Panel :
		public BoxComponent
	{
	public:
		static constexpr ComponentTypeID TYPE_ID = 8;
		const ComponentTypeID getTypeID() const override { return TYPE_ID; }

		Panel(unsigned int width, unsigned int height);
		Panel(std::istream& is);
		Panel() {}
		~Panel();
		// Save & Load operator
		void serialize(std::ostream& os) const;
		void deserialize(std::istream& is);

		std::vector<GraphicComponent*> getGraphicChildren();

	public:
		void init();
		void end();
	private:
		ComponentHandle transformHandle;
		ChildCollection<GraphicComponent> graphicChildren;
	};
}
#endif