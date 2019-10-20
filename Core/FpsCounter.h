#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H
namespace Core {
	class Text;
	class FpsCounter
	{
	public:
		FpsCounter();
		~FpsCounter();

		void tick(float deltaTime, Text* display = nullptr);
	private:
		// FPS counter variables
		unsigned int fpsCounter = 0;
		float fpsPrintTimer = 0;

		unsigned int currentFPS;
	};
}
#endif