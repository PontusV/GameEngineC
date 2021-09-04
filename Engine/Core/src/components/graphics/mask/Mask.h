#ifndef MASK_H
#define MASK_H
#include "entity/component/IComponentData.h"
#include <cstddef>
#include "Mask.generated.h"

namespace Core {

	CLASS() Mask : public IComponentData {
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