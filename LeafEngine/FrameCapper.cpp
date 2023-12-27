#include "FrameCapper.h"

unsigned int FrameCapper::fps = 60;

FrameCapper::FrameCapper() {
	first = std::chrono::steady_clock::now();
}

FrameCapper::~FrameCapper() {
	Colision::DealWithColisions();
	last = std::chrono::steady_clock::now();
	duration = last - first;
	if (duration.count() * 1000.0 < 1000.0 / fps) {
		std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000.0 / fps - duration.count())));
	}
}

void FrameCapper::SetFrameCap(unsigned int fps)
{
	FrameCapper::fps = fps;
}

unsigned int FrameCapper::GetFrameCap()
{
	return fps;
}

float FrameCapper::GetDeltaTime()
{
	return 1.0f / FrameCapper::GetFrameCap(); //1.f (not 1000.f because delta time needs to be in respect with seconds)
}
