#include "FpsCounter.h"
#include <iostream>
#include <sstream>
#include "Text.h"
using namespace Core;

FpsCounter::FpsCounter()
{
}


FpsCounter::~FpsCounter()
{
}

void FpsCounter::tick(float deltaTime, Text* display) {
	fpsCounter++;
	fpsPrintTimer += deltaTime;
	if (fpsPrintTimer > 1.0f) {
		fpsPrintTimer -= 1.0f;
		std::cout << "Fps: " << fpsCounter << "\n";
		if (display) {
			std::stringstream ss;
			ss << "Fps: " << fpsCounter;
			display->setText(ss.str());
		}

		fpsCounter = 0;
	}
}