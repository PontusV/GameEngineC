#ifndef CAMERA_H
#define CAMERA_H
#include "Maths/Vector2.h"
#include "Maths/Matrix4.h"
namespace Core {
	class Window;

	class Camera {
	public:
		Camera(Window* window);
		~Camera();

		const Vector2& getPosition();
		Matrix4 getViewMatrix();

		void setPosition(Vector2 vec);
		void moveX(float value);
		void moveY(float value);
		void setRotation(float value);
		void rotate(float value);

	private:
		Vector2 position = Vector2(0.0f, 0.0f);
		float rotation = 0.0f;
		Matrix4 matrix;

		Window* window;
	};
}
#endif