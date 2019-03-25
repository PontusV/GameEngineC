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
		Panel();
		~Panel();

		void init();
		void end();

		std::vector<GraphicComponent*> getGraphicChildren();
		
		// Serializable
		virtual void		serialize(std::ostream& os) const;
		virtual void		deserialize(std::istream& is);

	private:
		ComponentHandle transformHandle;
		ChildCollection<GraphicComponent> graphicChildren;
	};
}
#endif