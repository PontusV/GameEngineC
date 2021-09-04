#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "entity/component/IComponentData.h"
#include "maths/Matrix4.h"
#include "maths/Vector2.h"

namespace Core {

	#include "Transform.generated.h"

	CLASS() Transform : public IComponentData {
		GENERATED_BODY()
	public:
		Transform(float x, float y, float z = 0.0f, float rotation = 0.0f, float scale = 1.0f);
		Transform();
		virtual ~Transform();

		void rotate(float radians);
		void moveX(float value);
		void moveY(float value);

		void updateLocalToWorldMatrix(const Matrix4& model);
		void resetLocalToWorldMatrix();
		const Matrix4& getLocalToWorldMatrix() const;
		const Matrix4& getWorldToLocalMatrix() const;

		void updateLocalModelMatrix();
		/* Calculates and returns a new model matrix from local position, rotation and scale */
		const Matrix4& getLocalModelMatrix() const;

		/* Returns local rotation */
		const float& getLocalRotation() const;
		/* Returns position in local space. */
		const Vector2& getLocalPosition() const;
		/* Returns position in world space. */
		const Vector2 getPosition() const;
		const float& getZ() const;
		
		/* Sets the rotation in local space. */
		void setLocalRotation(float radians);
		/* Sets the position in local space. */
		void setLocalPosition(Vector2 pos);
		/* Sets local x coord */
		void setLocalX(float value);
		/* Sets local y coord */
		void setLocalY(float value);
		/* Sets the position in world space. */
		void setPosition(Vector2 pos);
		/* Sets x coord in world space. */
		void setX(float value);
		/* Sets y coord in world space. */
		void setY(float value);
		void setZ(float value);

		/* Marks this transform for an update */
		void setChanged();
		bool hasChanged();

	protected:
		Matrix4 localToWorldMatrix;		// World Model Matrix
		Matrix4 worldToLocalMatrix;		// Inverse of worldModelMatrix. Multiply with a world position to get local position
		Matrix4 localModelMatrix;		// Local Model Matrix
		PROPERTY(Update=setChanged())
		Vector2 position;				// Local position (Position in parent)
		PROPERTY()
		float z;						// Depth for drawing order
		PROPERTY(Update=setChanged())
		float rotation;					// Local rotation in radians
		PROPERTY(Update=setChanged())
		float scale;					// Local scale (both width and height atm)

		bool changed = true;			// Defines if the transform has moved
	};
}
#endif