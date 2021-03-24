#ifndef CAMERA_H
#define CAMERA_H
#include <Core/Camera.h>
#include "maths/Vector2.h"
#include "maths/Matrix4.h"
namespace Core {
	class Window;

	class Camera: public ICamera {
	public:
		Camera(Window* window);
		~Camera();

		const Vector2& getPosition();
		const Matrix4& getViewMatrix() const;
		void updateViewMatrix();

		float getX();
		float getY();
		void setPosition(float x, float y);
		void setPosition(Vector2 vec);
		void moveX(float value);
		void moveY(float value);
		void setRotation(float value);
		void rotate(float value);

		const Matrix4& getWorldToScreenMatrix() const;

	private:
		Vector2 position = Vector2(0.0f, 0.0f);
		float rotation = 0.0f;
		Matrix4 viewMatrix;
		Matrix4 worldToScreenMatrix;

		Window* window;
	};
}
#endif