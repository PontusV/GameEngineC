#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "entity/component/Component.h"
#include "graphics/data/Color.h"
#include "graphics/data/Shader.h"
#include <cstddef>

#include "Graphic.generated.h"

namespace Core {
	CLASS() Graphic : public Component {
		GENERATED_BODY()
	public:
		virtual ~Graphic() = 0;

		void setColor(Color value);
		const Color& getColor() const;

		void setShader(Shader value);
		const Shader& getShader() const;

		void setSortingOrder(std::size_t value);
		const std::size_t& getSortingOrder() const;
	protected:
		Graphic(std::size_t sortingOrder, Shader shader, Color color);
		Graphic();
	private:
		PROPERTY()
		Color color;
		PROPERTY()
		Shader shader;
		PROPERTY()
		std::size_t sortingOrder;
	};
}

#endif