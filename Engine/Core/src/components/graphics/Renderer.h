#ifndef RENDERER_H
#define RENDERER_H

#include "entity/component/Component.h"
#include "Renderer.generated.h"

namespace Core {
	CLASS() Renderer : public Component {
		GENERATED_BODY()
	public:
		virtual ~Renderer() = 0;

		bool isEnabled() const;
		void enable();
		void disable();

	protected:
		Renderer();
	private:
		PROPERTY()
		bool enabled = true;
	};
}
#endif