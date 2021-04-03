#ifndef MASK_H
#define MASK_H
#include "entity/component/Component.h"
#include <cstddef>
#include "Mask.generated.h"

namespace Core {

	CLASS() Mask : public Component {
		GENERATED_BODY()
	public:
		Mask();
		~Mask();

		bool isEnabled();
		void enable();
		void disable();

	private:
		PROPERTY()
		bool enabled = true;
	};
}
#endif