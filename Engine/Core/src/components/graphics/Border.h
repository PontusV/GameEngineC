#ifndef BORDER_COMPONENT_H
#define BORDER_COMPONENT_H

#include "Graphic.h"
#include <cstddef>
#include "Border.generated.h"

namespace Core {
	CLASS() Border : public Graphic {
		GENERATED_BODY()
	public:

		Border(std::size_t thickness, Color color, bool inner = false, bool top = true, bool right = true, bool bottom = true, bool left = true);
		Border();
		~Border();

		bool sideEnabled(std::size_t side) const;
		bool isInner() const;
		void setInner(bool value);
		const std::size_t& getBorderThickness() const;

		void setPadding(std::size_t value);
		const std::size_t& getPadding() const;

	private:
		PROPERTY()
		bool top = true;
		PROPERTY()
		bool right = true;
		PROPERTY()
		bool bottom = true;
		PROPERTY()
		bool left = true;
		PROPERTY()
		bool inner = false;
		PROPERTY()
		std::size_t borderThickness = 1;
		PROPERTY()
		std::size_t padding = 0;
	};
}
#endif